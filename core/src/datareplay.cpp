#include "datareplay.h"

DataReplay::DataReplay(/* args */)
{
    initZmq();
}

DataReplay::~DataReplay()
{
}

double DataReplay::createIndexInfo(const std::string &fileName)
{
    ifs_.open(fileName);
    if (!ifs_.is_open())
    {
        std::cout << fileName << " can not open!!!\n";
        return -1;
    }
    replayDatas_.clear();
    ifs_.seekg(0, std::ios::end);
    ifs_.seekg(0);

    // read file_head
    KDataFileHead fileHead;
    ifs_.read((char *)&fileHead, sizeof(fileHead));
    std::string protoContent;
    protoContent.resize(fileHead.protoContentSize);
    ifs_.read(&protoContent[0], protoContent.size());

    std::string comment;
    if (fileHead.commentLength > 0)
    {
        comment.resize(fileHead.commentLength);
        ifs_.read(&comment[0], fileHead.commentLength);
    }

    ReplayData replayData;
    while (ifs_.read((char *)&replayData.head, sizeof(replayData.head)))
    {
        replayData.dataPos = ifs_.tellg();
        ifs_.seekg(replayData.head.size, std::ios::cur);
        replayDatas_.insert({replayData.head.timestamp, replayData});
    }

    replayIter_ = replayDatas_.begin();
    ifs_.clear(); //清除错误标志
    ifs_.seekg(0);
    isReplaying = true;
    return (--replayDatas_.end())->first - replayDatas_.begin()->first;
}

void DataReplay::beginReplay()
{
    double firstTime = 0;
    int num = 0;
    while (isReplaying)
    {
        if (replayIter_ != replayDatas_.end())
        {
            if (timeGap_ == -1)
            {
                timeGap_ = KTime().getTime() - replayIter_->first;
                firstTime = replayIter_->first;
            }
            std::string name = replayIter_->second.head.name;
            double timestamp = replayIter_->second.head.timestamp;
            size_t dataPos = replayIter_->second.dataPos;
            ifs_.seekg(dataPos);
            std::string mdata;
            int size = replayIter_->second.head.size;
            mdata.resize(size);
            ifs_.read(&mdata[0], size);

            int sleepTime = (timestamp + timeGap_ - KTime().getTime()) * 1e3;
            if (sleepTime > 1)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
            }
            bool is_register = replayTopics.find(name) != replayTopics.end();
            if (is_register)
            {
                // iacc active
                if (iaccActive >= 0 && name == "sab_c385-VcuProto.BusVcuTransmission" && timestamp - firstTime > iaccActive && num <= 200)
                {
                    //active twice
                    if (num < 20 || num > 180 || (num > 60 && num < 80) || (num > 120 && num < 140))
                        mdata = modifySignalVal("VcuProto.BusVcuTransmission", mdata, "vcu_shift_lvl_posn", 4);
                    num++;
                    // qDebug() << "vcu_shift:" << num << " iaccActive:" << iaccActive;
                }
                // if(name.find("sab_c385")!=-1 && replayTopics_TC397.find(name) != replayTopics_TC397.end())
                // {
                //     name.replace(name.find("sab_c385"),8,"reinjection");
                // }
                if (replayTopics_TC397.find(name) != replayTopics_TC397.end())
                {
                    name.replace(0, name.find("-"), "reinjection");
                }
                publish(mdata, name); //发送到解析程序中
            }
            replayIter_++;
        }
        else
        {
            ifs_.close();
            timeGap_ = -1;
            break;
        }
    }
    ifs_.close();
    timeGap_ = -1;
}

void DataReplay::initZmq()
{
    // std::string address = "tcp://127.0.0.1:7890";
    std::string address = "ipc:///tmp/replayer.ipc";
    socket_ = new zmq::socket_t(context_, zmq::socket_type::pub);
    socket_->bind(address);
    std::cout << "replayer bind ipc:///tmp/replayer.ipc" << std::endl;

    std::string out_pub_address = "tcp://127.0.0.1:8888";
    out_pub_socket_ = new zmq::socket_t(out_pub_context_, zmq::socket_type::pub);
    out_pub_socket_->connect(out_pub_address);
    std::cout << "replayer connect tcp://127.0.0.1:8888" << std::endl;
}

void DataReplay::publish(const google::protobuf::Message &data, const std::string &topicName)
{
    std::string mdata;
    data.SerializeToString(&mdata);
    socket_->send(zmq::message_t(topicName), zmq::send_flags::sndmore);
    socket_->send(zmq::message_t(std::to_string(KTime::getMonotonicTime())), zmq::send_flags::sndmore);
    socket_->send(mdata.data(), mdata.length());

    out_pub_socket_->send(zmq::message_t(topicName), zmq::send_flags::sndmore);
    out_pub_socket_->send(zmq::message_t(std::to_string(KTime::getMonotonicTime())), zmq::send_flags::sndmore);
    out_pub_socket_->send(mdata.data(), mdata.length());
}

void DataReplay::publish(const std::string &data, const std::string &topicName)
{
    socket_->send(zmq::message_t(topicName), zmq::send_flags::sndmore);
    socket_->send(zmq::message_t(std::to_string(KTime::getMonotonicTime())), zmq::send_flags::sndmore);
    socket_->send(data.data(), data.length());

    out_pub_socket_->send(zmq::message_t(topicName), zmq::send_flags::sndmore);
    out_pub_socket_->send(zmq::message_t(std::to_string(KTime::getMonotonicTime())), zmq::send_flags::sndmore);
    out_pub_socket_->send(data.data(), data.length());
}
