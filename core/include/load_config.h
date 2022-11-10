#ifndef LOAD_CONFIG_H
#define LOAD_CONFIG_H

#include <unistd.h>
#include "yaml-cpp/yaml.h"
#include <iostream>
#include <QDir>
#include <QStringList>
#include <QString>

struct EvalutionConfig
{
    std::string DTMNumber;
    std::string scriptPath;
    std::string scriptConfigPath;
};

struct CaseConfig{
    std::vector<std::string> signals_;
    std::string dataFile;
	std::vector<std::string> dataFiles;
    std::string author;
    double iaccActive = -1;

};

struct StartCapilot {
    bool isStartCapilot =  false;
    std::string CapilotScriptPath;
};

class LoadConfig
{
public:
    LoadConfig(){}
    ~LoadConfig(){}
    std::vector<EvalutionConfig> getEvalutionConfig(){
        return config_;
    }

    StartCapilot getStartCapilot(){
        return startCapilot;
    }

    std::set<std::string> getReplayTopics();
    std::set<std::string> getReplayTopics_TC397();

    void loadReinjectConfig(const std::string & reinjectConfigFile);

    CaseConfig loadCaseConfig(const std::string & caseConfigFile);

private:
	void findnameInDir(std::vector<std::string> & fileNames, const QString & path);
	
    std::vector<EvalutionConfig> config_;

    std::vector<std::string> replayTopic;
    std::vector<std::string> replayTopic_TC397;

    StartCapilot startCapilot;

};

#endif // LOAD_CONFIG_H

