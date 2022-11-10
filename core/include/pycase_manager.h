#ifndef PYCASE_MANAGER_H
#define PYCASE_MANAGER_H

#include <QObject>
#include <QDebug>
#include <thread>
#include <Python.h>
#include <QThread>
#include <QVector>
#include <QQueue>
#include <QMutex>
#include <QDir>
#include <QDateTime>
#include <fstream>
#include "load_config.h"
#include "ktime.h"

struct MsgData{
    QString signalName;
    double value;
    double timeStamp;
};

struct ResultInfo
{
    QString result;
    QString reason;
};

struct ConfigInfo{
    std::string scriptPath;
    std::string author;
    std::string dataFile;
    std::string DTMNumber;
    std::vector<std::string> caseSignals;
};

class pycaseManager : public QObject
{
    Q_OBJECT
public:
    explicit pycaseManager(QObject *parent = nullptr);
    ~pycaseManager();
    void initPythonEnv();
    void loadPythonClass(const char *name);
    void addData(double timestamp, QMap<QString, double> signal_map);
    void parserResult(QString caseName, QString result);
    // void parserResult(const std::string & caseName, const std::string & result){}

public:
    QMap<QString, EvalutionConfig> configMap_;// key:case名字，value:对应的配置信息
    QMap<QString, ResultInfo> resultMap_;

public:
    void run(const std::string & caseName, const double &);

    void setConfigInfo(const EvalutionConfig  &evalConf,const CaseConfig &caseConf, int current){
        configInfo_.scriptPath = evalConf.scriptPath;
        configInfo_.author = caseConf.author;
        configInfo_.dataFile = caseConf.dataFiles[current];
        configInfo_.DTMNumber = evalConf.DTMNumber;
        configInfo_.caseSignals = caseConf.signals_;
	}

private:
    ConfigInfo configInfo_;

    bool initFlagCpy = false;
    PyGILState_STATE gstate;
    PyObject* pInstance_ = NULL;
    QMutex mutex;
    QQueue<MsgData> dataQueue_;

    std::vector<std::string> scriptPaths_;
    bool isFirstWriteReport = true;


signals:
    void topicUpdate(QStringList);
    void sendDatapath(QString);

public slots:

};

#endif // PYCASE_MANAGER_H
