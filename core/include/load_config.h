#ifndef LOAD_CONFIG_H
#define LOAD_CONFIG_H

#include <unistd.h>
#include "yaml-cpp/yaml.h"
#include <iostream>

struct EvalutionConfig
{
    std::string DTMNumber;
    std::string scriptPath;
    std::string scriptConfigPath;
};

struct CaseConfig{
    std::vector<std::string> signals_;
    std::string dataFile;
    std::string author;
    double iaccActive = -1;

};

class LoadConfig
{
public:
    LoadConfig(){}
    ~LoadConfig(){}
    std::vector<EvalutionConfig> getEvalutionConfig(){
        return config_;
    }

    std::set<std::string> getReplayTopics();

    void loadReinjectConfig(const std::string & reinjectConfigFile);

    CaseConfig loadCaseConfig(const std::string & caseConfigFile);

private:
    std::vector<EvalutionConfig> config_;

    std::vector<std::string> replayTopic;

};

#endif // LOAD_CONFIG_H

