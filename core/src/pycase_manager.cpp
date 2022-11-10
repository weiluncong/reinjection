#include "pycase_manager.h"

#define TOQSTR(x) QString::fromStdString(x)
#define TOSTR(x) QString(x).toStdString()

pycaseManager::pycaseManager(QObject *parent) : QObject(parent)
{
    initPythonEnv();
}

pycaseManager::~pycaseManager()
{
    // int nHold = PyGILState_Check();
    // if (!nHold)
    // {
    //     gstate = PyGILState_Ensure();
    // }
    // Py_CLEAR(pModule_);
    // Py_CLEAR(pDict_);
    // Py_CLEAR(pConstruct_);
    // Py_CLEAR(pClassCalc_);
    // Py_CLEAR(pInstance_);
    // Py_Finalize();
}

void pycaseManager::initPythonEnv()
{
    Py_Initialize();
    int nHold = PyGILState_Check();
    if (!nHold)
    {
        gstate = PyGILState_Ensure();
    }

    if (Py_IsInitialized())
    {
        PyRun_SimpleString("import sys");
        PyRun_SimpleString("sys.path.append('./testpy/')");
        PyRun_SimpleString("sys.path.append('./testpy/case')");
    }
    else
    {
        qDebug() << QObject::tr("Python环境初始化失败...");
    }
    PyGILState_Release(gstate);
}

void pycaseManager::run(const std::string &caseName, const double &time_)
{
    int info = 0;
    PyObject *pRet;
    loadPythonClass(caseName.data());
    double startTime = KTime().getTime();

    while (initFlagCpy)
    {
        mutex.lock();
        while (!dataQueue_.isEmpty())
        {
            MsgData mData = dataQueue_.dequeue();
            pRet = PyObject_CallMethod(pInstance_, "addData", "sdd", mData.signalName.toStdString().data(), mData.value, mData.timeStamp);
            // 解析返回值
            PyArg_Parse(pRet, "i", &info);
            if (info == 1)
            {
                char *result;
                initFlagCpy = false;
                PyObject *pRet1 = PyObject_CallMethod(pInstance_, "getResult", "");
                PyArg_Parse(pRet1, "s", &result);
                Py_CLEAR(pRet1);
                parserResult(TOQSTR(caseName), QString(result));
                break;
            }
            Py_CLEAR(pRet);
        }
        mutex.unlock();

        if (KTime().getTime() - startTime > (time_ + 10) || KTime().getTime() - startTime > 180)
        {
            std::string timeoutResult = "failed;timeout " + std::to_string(time_);
            initFlagCpy = false;
            parserResult(TOQSTR(caseName), TOQSTR(timeoutResult));
            break;
        }
    }
    Py_Finalize();
    // PyGILState_Release(gstate);
}

void pycaseManager::loadPythonClass(const char *name)
{
    int nHold = PyGILState_Check();
    if (!nHold)
    {
        // qDebug()<<name<<"get gil";
        gstate = PyGILState_Ensure();
    }
    pModule_ = PyImport_ImportModule(name); //参数为Python脚本的文件名

    if (pModule_)
    {
        pDict_ = PyModule_GetDict(pModule_);
        if (!pDict_)
        {
            qDebug("Cant find dictionary./n");
            return;
        }
        pClassCalc_ = PyDict_GetItemString(pDict_, name);
        if (!pClassCalc_)
        {
            qDebug("Cant find calc class./n");
            return;
        }
        //得到构造函数而不是类实例
        pConstruct_ = PyInstanceMethod_New(pClassCalc_);
        if (!pConstruct_)
        {
            qDebug("Cant find calc construct./n");
            return;
        }
        //实例化类得到类对象
        pInstance_ = PyObject_CallObject(pConstruct_, NULL);

        for (auto sgl : configInfo_.caseSignals)
        {
            PyObject_CallMethod(pInstance_, "setSignals", "s", sgl.data());
            // std::cout<<sgl<<std::endl;
        }

        if (!PyGILState_Check())
        {
            gstate = PyGILState_Ensure();
        }
        initFlagCpy = true;
    }
    else
    {
        qDebug() << QObject::tr("导入Python模块失败...");
    }
}

void pycaseManager::parserResult(QString caseName, QString result)
{
    ResultInfo res;
    res.result = result.split(";").first();
    res.reason = result.split(";").last();
    if (res.result == "success" || res.result == "failed")
    {
        resultMap_.insert(caseName, res);
    }
    else
    {
        qDebug() << "error result";
        return;
    }
    std::map<std::string, std::string> currentResult;
    currentResult["scriptPath"] = configInfo_.scriptPath;
    // currentResult["DTMNumber"] = configInfo_.DTMNumber;
    // currentResult["dataFile"]= configInfo_.dataFile;
    // currentResult["author"] = configInfo_.author ;
    currentResult["result"] = TOSTR(res.result);
    currentResult["reason"] = TOSTR(res.reason);
    currentResult["dateTime"] = TOSTR(QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss"));

    emit UpdateReport(currentResult);

    QFile reportFile("report.yaml");
    if (reportFile.exists())
    {
        if (isFirstWriteReport)
        {
            isFirstWriteReport = false;
            reportFile.resize(0); // 第一个report，清空report内容
        }
    }
    else
    {
        if (!reportFile.open(QIODevice::WriteOnly))
        {
            throw std::runtime_error("error: cann't open report.yaml");
        }
        else
        {
            reportFile.close();
            isFirstWriteReport = false;
        }
    }

    YAML::Node result_yaml = YAML::LoadFile("report.yaml");

    std::vector<std::map<std::string, std::string>> detail;
    for (int i = 0; i < result_yaml["detail"].size(); ++i)
    {
        detail.push_back(result_yaml["detail"][i].as<std::map<std::string, std::string>>());
    }

    detail.push_back(currentResult);
    result_yaml["detail"] = detail;

    if (result_yaml["conclusion"].IsDefined())
    {
        if (currentResult["result"] != "success")
        {
            result_yaml["conclusion"] = currentResult["result"];
        }
    }
    else
    {
        result_yaml["conclusion"] = currentResult["result"];
    }

    std::ofstream ofs;
    ofs.open("report.yaml");
    if (!ofs.is_open())
    {
        throw std::runtime_error("report.yaml open failed!!!");
    }
    ofs << result_yaml;
    ofs.close();
}

void pycaseManager::addData(double timestamp, QMap<QString, double> signal_map)
{
    if (!initFlagCpy)
    {
        mutex.lock();
        dataQueue_.clear();
        mutex.unlock();
        return;
    }
    for (auto p : signal_map.keys())
    {
        MsgData mData;
        mData.timeStamp = timestamp;
        mData.signalName = p;
        mData.value = signal_map[p];
        mutex.lock();
        dataQueue_.enqueue(mData);
        mutex.unlock();
    }
}
