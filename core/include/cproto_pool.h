/*
 * @Author: error: git config user.name && git config user.email & please set dead value or install git
 * @Date: 2022-06-13 03:40:23
 * @LastEditors: error: git config user.name && git config user.email & please set dead value or install git
 * @LastEditTime: 2022-06-13 08:40:57
 * @FilePath: /newmain/src/data_center/data_pool/cproto_pool.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef CPROTO_POOL_H
#define CPROTO_POOL_H

#include <QObject>
#include <fstream>
#include <unistd.h>
#include <sys/stat.h>
#include <QDebug>
#include "google/protobuf/descriptor_database.h"
#include "google/protobuf/descriptor.pb.h"
#include "google/protobuf/compiler/importer.h"
#include "google/protobuf/dynamic_message.h"
#include "zmq.hpp"

class CProtoPool : public QObject
{
    Q_OBJECT
private:
    explicit CProtoPool()
    {
    }
    ~CProtoPool() {}
    CProtoPool(const CProtoPool &) = delete;
    CProtoPool &operator=(const CProtoPool &) = delete;
    static CProtoPool *proto_pool_;

public:
    static CProtoPool *GetCProtoPool()
    {
        if (proto_pool_ == NULL)
        {
            proto_pool_ = new CProtoPool;
        }
        return proto_pool_;
    }
    std::string getProtoContent()
    {
        return protoContent_;
    }
    void LoadContentProto();
    void LoadProtoContent(const std::string &content);
    void updataProtoContent(std::string serviceAddress);
    google::protobuf::Message *GetProtoMessage(const std::string &topic_name, const std::string &data);
    google::protobuf::Message *CreateMessage(const std::string &message_name);

public:
    google::protobuf::compiler::Importer *importer_ = nullptr;
    int interfaceVersion_ = 3;

private:
    // ProtoFileContent proto_content_;
    google::protobuf::DynamicMessageFactory factory_;
    std::map<std::string, google::protobuf::Message *> msgs_;
    // std::string serviceAddress_ = "tcp://172.16.31.8:7766";
    // std::string service821Address_ = "tcp://172.16.31.8:7766";
    std::string protoContent_;
};

#endif // CPROTO_POOL_H
