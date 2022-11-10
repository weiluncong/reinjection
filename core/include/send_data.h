#ifndef SEND_DATA_H
#define SEND_DATA_H

#include <fstream>
#include <iostream>
#include "structure.h"
#include <string>
#include <string.h>
#include <map>
#include "google/protobuf/descriptor_database.h"
#include "google/protobuf/descriptor.pb.h"
#include "google/protobuf/compiler/importer.h"
#include "google/protobuf/dynamic_message.h"
#include "google/protobuf/message_lite.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/message.h"

#include <thread>
#include "ktime.h"
#include "peak_can_sender.h"
#include "udp_sender.h"
#include "origin_data.pb.h"
#include "datareplay.h"
#include <QString>
#include <QStringList>
#include <QMap>

typedef uint32_t uint32;
typedef uint16_t uint16;
using namespace std;

using std::cout;
using std::endl;
using std::istringstream;
using std::map;
using std::string;

#define AC_PORT (51004)
#define FR_PORT (51002)
#define FC_PORT (51003)

#define AC_IP "172.16.31.8"
#define FR_IP "172.16.31.8"
#define FC_IP "172.16.31.8"

class Send_Data
{
public:
    Send_Data();
    ~Send_Data();
    QStringList Set_File_Name(const string &file);
    QStringList Send();
    QStringList Stop();

    bool stop_flag = false;

    void Clear();

    void setIaccActive();

private:
    void read_file(const std::string &file_);

    void parse_proto(const std::string &msg);

    void parse_eth(const std::string &msg, const std::string &tocpic_name);

    bool send_function(const std::string &);

    template <typename T1, typename T2>
    void send_function_(T1 sender, T2 data);
    
    void send_function_ch4(AbstractCanSender * sender, const map<double, CanData> & data);

    DataReplay * replayer = nullptr;

    PeakCanSender *send_1 = nullptr;
    PeakCanFdSender *send_2 = nullptr;
    PeakCanFdSender *send_3 = nullptr;
    PeakCanFdSender *send_4 = nullptr;
    PeakCanFdSender *send_5 = nullptr;

    UdpSender *sender_fc = nullptr;
    UdpSender *sender_ac = nullptr;
    UdpSender *sender_fr = nullptr;

    map<double, CanData> channel_1;
    map<double, CanData> channel_2;
    map<double, CanData> channel_3;
    map<double, CanData> channel_4;
    map<double, CanData> channel_5;
    map<double, std::string> data_ac;
    map<double, std::string> data_fr;
    map<double, std::string> data_fc;

    string file_name_;

    double start_time = 0;
    double end_time = 0;
    QMap<int, QVector<int>> ids_;
    int num17f = 0;

    ofstream ofs_;
    bool active_iacc_flag = false;
};

#endif // SEND_DATA_H
