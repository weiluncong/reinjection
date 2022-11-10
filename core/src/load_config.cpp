#include "load_config.h"

// need modify
std::set<std::string> LoadConfig::getReplayTopics(){
    std::set<std::string> tmp;
        for(auto topic : replayTopic)
            tmp.insert(topic);
        return tmp;
}

void LoadConfig::loadReinjectConfig(const std::string & reinjectConfigFile){
    if(access(reinjectConfigFile.c_str(), R_OK) == -1){
        std::cout << "reinject config file can not open!!!\n";
        return;
    }

    YAML::Node yaml_ = YAML::LoadFile(reinjectConfigFile);
    const auto &case1 = yaml_["case"];
    for (size_t i = 0; i < case1.size(); i++)
    {
        const auto &case_ = case1[i];
        if(!case_["isReplay"].as<bool>())
            continue;
        EvalutionConfig tmp;
        tmp.DTMNumber = case_["DTMNumber"].as<std::string>();
        tmp.scriptPath = case_["scriptPath"].as<std::string>();
        tmp.scriptConfigPath = case_["scriptConfigPath"].as<std::string>();
        if(tmp.scriptConfigPath.find("/") != -1)
            tmp.scriptConfigPath = tmp.scriptConfigPath.substr(tmp.scriptConfigPath.find_last_of("/"), tmp.scriptConfigPath.size());
        config_.push_back(tmp);
    }
    replayTopic = yaml_["replayTopic"].as<std::vector<std::string>>();

}

CaseConfig LoadConfig::loadCaseConfig(const std::string & caseConfigFile){
    CaseConfig caseConfig_;
    if(access(caseConfigFile.c_str(), R_OK) == -1){
        std::cout << "case config file can not open!!!\n";
        return caseConfig_;
    }
    YAML::Node yaml_ = YAML::LoadFile(caseConfigFile);
    caseConfig_.signals_ = yaml_["Message"].as<std::vector<std::string>>();
    caseConfig_.author = yaml_["author"].as<std::string>();
    caseConfig_.dataFile = yaml_["dataFile"].as<std::string>();
    // if(caseConfig_.dataFile.find("/") != -1)
    //     caseConfig_.dataFile = caseConfig_.dataFile.substr(caseConfig_.dataFile.find_last_of("/")+1, caseConfig_.dataFile.size());
    if(yaml_.size() >= 4){
        caseConfig_.iaccActive = yaml_["IaccActive"].as<double>();
    }
    return caseConfig_;
}
