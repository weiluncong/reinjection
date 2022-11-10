#ifndef DATAREPLAY_H
#define DATAREPLAY_H

#include "structure.h"
#include <fstream>
#include <map>
#include "ktime.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <thread>
#include <set>
#include "zmq.hpp"
#include <google/protobuf/message.h>


#define MAX_NAME_SIZE 100

#pragma pack(1)
// struct RawDataHead
// {
//     char name[MAX_NAME_SIZE];
//     size_t size;
//     double timestamp;
// };

struct ReplayData {
    RawDataHead head;
    size_t dataPos;
};

// struct KDataFileHead
// {
//     float version;
//     size_t protoContentSize;
//     uint16_t commentLength;
//     char reserve[200];
// };

#pragma pack()

class DataReplay {
private:
    std::multimap<double, ReplayData> replayDatas_;
    std::multimap<double, ReplayData>::iterator replayIter_;

    std::ifstream ifs_;

    zmq::socket_t* out_pub_socket_ = nullptr;
    zmq::context_t out_pub_context_;

    zmq::socket_t* socket_ = nullptr;
    zmq::context_t context_;

    std::set<std::string> replayTopics;

    double timeGap_ = -1;
    bool isReplaying = true;

    void initZmq();
    void publish(const google::protobuf::Message& data, const std::string& topicName);
    void publish(const std::string& data, const std::string& topicName);
   
public:
    DataReplay(/* args */);
    ~DataReplay();

    bool createIndexInfo(const std::string & fileName);

    void beginReplay();

    void setreplayTopics(const std::set<std::string> & topics){
        // for(auto topic : topics)
        //     std::cout << "topic : " << topic;
        replayTopics = topics;
    }

    void Stop(){
        isReplaying = false;
    }
};



#endif // DATAREPLAY_H