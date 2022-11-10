#ifndef ORIGIN_DATA_H
#define ORIGIN_DATA_H

#include <string>
#include <iostream>
#include <sstream>
#include "cproto_pool.h"
#include "google/protobuf/message.h"
#include "google/protobuf/descriptor_database.h"
#include "google/protobuf/descriptor.pb.h"
#include "google/protobuf/compiler/importer.h"
#include "google/protobuf/dynamic_message.h"

using namespace std;





class OriginCanData
{

public:
    OriginCanData();
    ~OriginCanData();

    void ParseFromString(const std::string &mdata);

    double id(){return id_;}
    double raw_timestamp(){return raw_timestamp_;}
    double time_stamp(){return time_stamp_;}
    string can_data(){return can_data_;}
    double data_len(){return data_len_;}
    double bus_id(){return bus_id_;}

private:
    double id_ = 1;
    double raw_timestamp_ = 2;
    double time_stamp_ = 3;
    string can_data_;
    double data_len_ = 5;
    double bus_id_ = 6;

    CProtoPool *proto_pool_ = nullptr;

template <class T>
T stringToNum(const std::string &str)
{
    std::istringstream iss(str);
    T num;
    iss >> num;
    return num;
}

};

class OriginUdpData
{

public:
    OriginUdpData();
    ~OriginUdpData();

    void ParseFromString(const std::string &mdata);

    double sensor_type(){return sensor_type_;}
    double time_stamp(){return time_stamp_;}
    string udp_data(){return udp_data_;}

private:
    double sensor_type_ = 1;
    double time_stamp_ = 2;
    string udp_data_;

    CProtoPool *proto_pool_ = nullptr;

template <class T>
T stringToNum(const std::string &str)
{
    std::istringstream iss(str);
    T num;
    iss >> num;
    return num;
}

};

#endif // ORIGIN_DATA_H