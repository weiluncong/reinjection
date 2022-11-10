#include "readevalutionyaml.h"

ReadEvalutionYaml::ReadEvalutionYaml()
{
    if(access("./dbc/eval.yaml", R_OK) == -1){
        std::cout << "config file can not open!!!";
        return;
    }
    YAML::Node yaml_ = YAML::LoadFile("./dbc/eval.yaml");
    config.DTMNumber = yaml_["CaseName"].as<std::string>();
    config.SignalNames = yaml_["SignalNames"].as<std::vector<std::string>>();
    config.referenceTopic = yaml_["referenceTopic"].as<std::string>();
    
}