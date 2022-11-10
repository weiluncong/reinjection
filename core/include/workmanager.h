#ifndef WORKMANAGER_H
#define WORKMANAGER_H

#include "send_data.h"
#include <dirent.h>

class WorkManager
{
public:
    WorkManager();

    void run();

private:
    void read_file_in_dir(const char *p_dir_name, std::vector<std::string> &file_names);
};

#endif // WORKMANAGER_H
