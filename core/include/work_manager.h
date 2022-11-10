#ifndef WORK_MANAGER_H
#define WORK_MANAGER_H

#include "load_config.h"
#include "datareplay.h"
#include "receiveparserdata.h"
#include "pycase_manager.h"
#include <thread>

class WorkManager : public QObject
{
    Q_OBJECT
private:
    /* data */

    void runCapilot(std::string capilotPath, std::string capilotScriptName);
public:
    WorkManager(/* args */);
    ~WorkManager();

    void run();


};


#endif // WORK_MANAGER_H


