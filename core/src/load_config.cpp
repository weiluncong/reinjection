#include "load_config.h"
#include <QDebug>

// need modify
std::set<std::string> LoadConfig::getReplayTopics()
{
    std::set<std::string> tmp;
    for (auto topic : replayTopic)
        tmp.insert(topic);
    return tmp;
}
std::set<std::string> LoadConfig::getReplayTopics_TC397()
{
    std::set<std::string> tmp;
    for (auto topic : replayTopic_TC397)
        tmp.insert(topic);
    return tmp;
}

void LoadConfig::loadReinjectConfig(const std::string &reinjectConfigFile)
{
    if (access(reinjectConfigFile.c_str(), R_OK) == -1)
    {
        std::cout << "reinject config file can not open!!!\n";
        return;
    }

    std::cout << "loading reinjectConfigFile:" << reinjectConfigFile << std::endl;
    YAML::Node yaml_ = YAML::LoadFile(reinjectConfigFile);

    startCapilot.isStartCapilot = yaml_["isStartCapilot"].as<bool>();
    startCapilot.CapilotScriptPath = yaml_["CapilotScriptPath"].as<std::string>();

    const auto &case1 = yaml_["case"];
    for (size_t i = 0; i < case1.size(); i++)
    {
        const auto &case_ = case1[i];
        if (!case_["isReplay"].as<bool>())
            continue;
        EvalutionConfig tmp;
        tmp.DTMNumber = case_["DTMNumber"].as<std::string>();
        tmp.scriptPath = case_["scriptPath"].as<std::string>();
        tmp.scriptConfigPath = case_["scriptConfigPath"].as<std::string>();
        if (tmp.scriptConfigPath.find("/") != -1)
            tmp.scriptConfigPath = tmp.scriptConfigPath.substr(tmp.scriptConfigPath.find_last_of("/"), tmp.scriptConfigPath.size());
        config_.push_back(tmp);
    }
    replayTopic = yaml_["replayTopic"].as<std::vector<std::string>>();
    replayTopic_TC397 = yaml_["replayTopic_TC397"].as<std::vector<std::string>>();
    replayTopic.insert(replayTopic.end(), replayTopic_TC397.begin(), replayTopic_TC397.end());
}

CaseConfig LoadConfig::loadCaseConfig(const std::string &caseConfigFile)
{
    CaseConfig caseConfig_;
    if (access(caseConfigFile.c_str(), R_OK) == -1)
    {
        std::cout << "case config file can not open!!!\n";
        return caseConfig_;
    }
    std::cout << "loading caseConfigFile:" << caseConfigFile << std::endl;
    YAML::Node yaml_ = YAML::LoadFile(caseConfigFile);
    caseConfig_.signals_ = yaml_["Message"].as<std::vector<std::string>>();
    caseConfig_.author = yaml_["author"].as<std::string>();
    caseConfig_.dataFile = yaml_["dataFile"].as<std::string>();

    if (caseConfig_.dataFile.find(".dat") == -1)
    {
        findnameInDir(caseConfig_.dataFiles, QString::fromStdString(caseConfig_.dataFile));
    }
    else
    {
        if (caseConfig_.dataFile.find("/") != -1)
        {
            caseConfig_.dataFile = caseConfig_.dataFile.substr(caseConfig_.dataFile.find_last_of("/") + 1, caseConfig_.dataFile.size());
        }
        caseConfig_.dataFile = "./data/" + caseConfig_.dataFile;
        caseConfig_.dataFiles.push_back(caseConfig_.dataFile);
    }
    caseConfig_.iaccActive = yaml_["IaccActive"].as<double>();
    return caseConfig_;
}

void LoadConfig::findnameInDir(std::vector<std::string> &fileNames, const QString &path)
{
    QDir dir(path);
    if (!dir.exists())
        return;

    // 查看路径中后缀为.dat格式的文件
    QStringList filters;
    filters << QString("*.dat");
    dir.setFilter(QDir::Files | QDir::NoSymLinks); //设置类型过滤器，只为文件格式
    dir.setNameFilters(filters);                   //设置文件名称过滤器，只为filters格式

    //统计cfg格式的文件个数
    int dir_count = dir.count();
    if (dir_count <= 0)
        return;

    //存储文件名称
    QStringList string_list;
    for (int i = 0; i < dir_count; i++)
    {
        QString file_name = path + "/" + dir[i]; //文件名称
        qDebug() << "file_name" << file_name;
        fileNames.push_back(file_name.toStdString());
    }
}