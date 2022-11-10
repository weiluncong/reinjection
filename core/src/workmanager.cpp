#include "workmanager.h"

WorkManager::WorkManager()
{
    run();
}

void WorkManager::run()
{
    Send_Data *send_data = new Send_Data();
    std::vector<std::string> file_names_;
    read_file_in_dir("/home/ca/data/test", file_names_);
    int i = 0;
    while (1)
    {
        for (auto file_name : file_names_)
        {
            send_data->Set_File_Name(file_name);
            cout << i++ << " times" << "\n";
        }
    }
}

void WorkManager::read_file_in_dir(const char *p_dir_name, std::vector<std::string> &file_names)
{
    DIR *p_dir = opendir(p_dir_name);
    if (p_dir == nullptr)
    {
        throw std::runtime_error("dir error !!!");
    }

    struct dirent *p_file = nullptr;
    while ((p_file = readdir(p_dir)) != nullptr)
    {
        std::string cur_file_name = p_file->d_name;
        if (cur_file_name.find(".dat") != -1)
        {
            std::string tmp = p_dir_name;
            file_names.push_back(tmp + "/" + cur_file_name);
            std::cout << cur_file_name << std::endl;
        }
    }
    closedir(p_dir);
}
