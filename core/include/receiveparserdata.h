#ifndef RECEIVE_PARSERDATA_H
#define RECEIVE_PARSERDATA_H
#include <string>
#include "cproto_pool.h"
#include "google/protobuf/descriptor_database.h"
#include "google/protobuf/descriptor.pb.h"
#include "google/protobuf/compiler/importer.h"
#include "google/protobuf/dynamic_message.h"
#include <zmq.hpp>
#include <thread>
#include <QDebug>
#include <QMap>
#include <QList>
#include <QObject>


#define TOQSTR(x) QString::fromStdString(x)
#define TOSTR(x) QString(x).toStdString()

template <class T>
T stringToNum(const std::string &str)
{
    std::istringstream iss(str);
    T num;
    iss >> num;
    return num;
}

class ReveiveParserData : public QObject
{
    Q_OBJECT
private:
    /* data */

    CProtoPool *proto_pool_ = nullptr;
    std::string protoAddressPc_ = "tcp://127.0.0.1:7766";

    QMap<QString, double> toSWCData(const google::protobuf::Message &msg, const QString &topicName);
    QString getRepeatedMsg(const google::protobuf::Message &msg,
                       const google::protobuf::FieldDescriptor *field,int index);
    void handleData(const double & timeStamp, const std::string & topicName, const std::string & data);

    QList<QString> needSignal;

    QVector<std::string> needReceiveTopics;

    bool isStop = false;
public:
    ReveiveParserData(/* args */);
    ~ReveiveParserData();

    // void init(const std::vector<std::string>  & signal);
    void init();
    void setSignalFilter(const std::vector<std::string>  & signal);
    void receiveData();

    void setStop(){isStop = true;}
signals:
    void dataUpdate(const double, QMap<QString, double>);
    
};



#endif // RECEIVE_PARSERDATA_H