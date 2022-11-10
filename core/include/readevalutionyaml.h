#include "unistd.h"
#include "yaml-cpp/yaml.h"
#include <iostream>
#include <map>
#include <QString>
#include <string>
#include <QStringList>

using std::vector;

struct EvalConfig{
    std::string DTMNumber;
    std::string referenceTopic;
    std::vector<std::string> SignalNames;
};

class ReadEvalutionYaml
{
private:
    /* data */
public:
    ReadEvalutionYaml();
    ~ReadEvalutionYaml();

    EvalConfig getEvalConfig(){
        return config;
    }
private:

    EvalConfig config;
};




