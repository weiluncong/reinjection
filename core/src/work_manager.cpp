#include "work_manager.h"

WorkManager::WorkManager(/* args */)
{
}

WorkManager::~WorkManager()
{
}

void WorkManager::run()
{
    LoadConfig loadConfig;
    loadConfig.loadReinjectConfig("./ReinjectConfig.yaml");
    auto config_ = loadConfig.getEvalutionConfig();
    auto replayTopics = loadConfig.getReplayTopics();
    auto replayTopics_TC397 = loadConfig.getReplayTopics_TC397();
    auto startCapilot = loadConfig.getStartCapilot();

    DataReplay *replayer = new DataReplay();
    replayer->setreplayTopics(replayTopics);
    replayer->setreplayTopics_TC397(replayTopics_TC397);

    ReveiveParserData *reveiveParserData = nullptr;
    pycaseManager *pyManager = new pycaseManager();

    qRegisterMetaType<QMap<QString, double>>("QMap<QString,double>");

    int number = 1;

    for (auto config : config_)
    {
        auto caseConfig = loadConfig.loadCaseConfig("./testpy/config" + config.scriptConfigPath);
        int current_num = 0;
        for (auto dataFile : caseConfig.dataFiles)
        {
            if (startCapilot.isStartCapilot)
            {
                std::cout << "waitting for capilot..." << std::endl;
                // system("killall capilot");
                system("./killcapilot.sh");
                std::this_thread::sleep_for(std::chrono::milliseconds(2 * 1000));
                std::string capilotPath = startCapilot.CapilotScriptPath.substr(0, startCapilot.CapilotScriptPath.find_last_of("/"));
                std::string capilotScriptName = startCapilot.CapilotScriptPath.substr(capilotPath.size() + 1, startCapilot.CapilotScriptPath.size());
                std::thread tt(&WorkManager::runCapilot, this, capilotPath, capilotScriptName);
                std::this_thread::sleep_for(std::chrono::milliseconds(30 * 1000));
                tt.detach();
            }
            std::cout << "start running test case." << std::endl;
            if (reveiveParserData == nullptr)
            {
                reveiveParserData = new ReveiveParserData();
                connect(reveiveParserData, &ReveiveParserData::dataUpdate, pyManager, &pycaseManager::addData, Qt::ConnectionType::DirectConnection);
            }

            std::cout << "current case: " << config.scriptPath << " " << number++ << " " << config_.size() << std::endl;

            qDebug() << "dataFile : " << QString::fromStdString(dataFile);
            pyManager->setConfigInfo(config, caseConfig, current_num++);
            if (caseConfig.signals_.empty())
            {
                pyManager->parserResult(QString::fromStdString(config.scriptPath), "failed;config file lack Message!!!!");
                continue;
            }

            double datTimeTotal = replayer->createIndexInfo(dataFile);
            replayer->setIaccActiveTime(caseConfig.iaccActive);

            if (datTimeTotal <= 0)
            {
                pyManager->parserResult(QString::fromStdString(config.scriptPath), "failed;dat file can not open or dat time error!!!");
                continue;
            }

            reveiveParserData->setSignalFilter(caseConfig.signals_);

            std::thread t1(&pycaseManager::run, pyManager, config.scriptPath, datTimeTotal);
            std::thread t2(&ReveiveParserData::receiveData, reveiveParserData);
            std::thread t3(&DataReplay::beginReplay, replayer);
            t1.join();
            reveiveParserData->setStop();
            replayer->Stop();
            t2.join();
            t3.join();
        }
    }
    system("./killcapilot.sh"); // 结束后killcapilot
    std::ofstream ofs_;
    ofs_.open("over");
    ofs_.close();
}

void WorkManager::runCapilot(std::string capilotPath, std::string capilotScriptName)
{
    system(std::string("cd " + capilotPath + " && ./" + capilotScriptName + " >/dev/null").data());
}
