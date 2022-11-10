#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <string>

typedef std::string MetaData;

#pragma pack(1)
struct KDataFileHead
{
    float version;
    size_t protoContentSize;
    uint16_t commentLength;
    char reserve[200];
};
#pragma pack()

#pragma pack(1)
struct RawDataHead
{
    char name[100];
//    unsigned int size;
    size_t size;
    double timestamp;
};
#pragma pack()

struct cReplayData
{
    RawDataHead head;
    MetaData mdata;
};


#pragma pack(1)
struct OriginCanData_
{
    int32_t id;
    int32_t raw_timestamp;
    double time_stamp;
//    unsigned char can_data[64] = {0};
    std::string can_data;
};
#pragma pack()

#endif // STRUCTURE_H
