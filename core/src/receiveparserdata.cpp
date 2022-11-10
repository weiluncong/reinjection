#include "receiveparserdata.h"

static std::string FieldTypeConvert(const google::protobuf::Message &msg, const google::protobuf::FieldDescriptor *field)
{
    std::ostringstream os;
    switch (field->cpp_type())
    {
    case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
        os << msg.GetReflection()->GetEnumValue(msg, field);
        break;
    case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
        os << msg.GetReflection()->GetInt32(msg, field);
        break;
    case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
        os << msg.GetReflection()->GetInt64(msg, field);
        break;
    case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
        os << msg.GetReflection()->GetUInt32(msg, field);
        break;
    case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
        os << msg.GetReflection()->GetUInt64(msg, field);
        break;
    case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
        os << std::fixed << msg.GetReflection()->GetDouble(msg, field);
        break;
    case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
        os << msg.GetReflection()->GetFloat(msg, field);
        break;
    case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
        os << msg.GetReflection()->GetBool(msg, field);
        break;
    case google::protobuf::FieldDescriptor::CPPTYPE_STRING:
        os << msg.GetReflection()->GetString(msg, field);
        break;
    default:
        break;
    }

    return os.str();
};

ReveiveParserData::ReveiveParserData(/* args */)
{
    init();
}

ReveiveParserData::~ReveiveParserData()
{
}

void ReveiveParserData::init()
{
    std::cout << "reveiveParserData init start" << std::endl;
    proto_pool_ = CProtoPool::GetCProtoPool();
    proto_pool_->updataProtoContent(protoAddressPc_);
    proto_pool_->LoadProtoContent(proto_pool_->getProtoContent());
    std::cout << "reveiveParserData init over" << std::endl;
}

void ReveiveParserData::setSignalFilter(const std::vector<std::string> &signal_)
{
    for (auto name : signal_)
    {
        needSignal.append(QString::fromStdString(name));
    }

    for (auto signal : signal_)
    {
        if (!needReceiveTopics.contains(signal.substr(0, signal.find_first_of("~"))))
            needReceiveTopics.push_back(signal.substr(0, signal.find_first_of("~")));
    }
    isStop = false;
}

void ReveiveParserData::receiveData()
{
    zmq::message_t msg;
    std::string topicName;
    double timeStamp;
    std::string address = "tcp://192.168.1.175:7890";
    // std::string address = "tcp://127.0.0.1:7890";
    zmq::context_t *context_ = new zmq::context_t();
    std::shared_ptr<zmq::socket_t> socket_ = std::shared_ptr<zmq::socket_t>(new zmq::socket_t(*context_, zmq::socket_type::sub));
    socket_->set(zmq::sockopt::subscribe, "");
    socket_->connect(address);
    while (!isStop)
    {
        socket_->recv(msg);
        topicName = msg.to_string();
        socket_->recv(msg); // receive timestamp
        std::istringstream is(msg.to_string());
        is >> timeStamp;
        socket_->recv(msg);
        if (needReceiveTopics.contains(topicName))
        {
            // std::cout << "topicName : " << topicName << std::endl;
            handleData(timeStamp, topicName, msg.to_string());
        }
    }
}

void ReveiveParserData::handleData(const double &timeStamp, const std::string &topicName, const std::string &data)
{

    std::string swc_name;
    std::string no_swc_name;
    std::string pack_name;
    QString sensorName = TOQSTR(topicName);
    QString swcname = TOQSTR(topicName);
    QStringList tempList = swcname.split("-");
    swc_name = TOSTR(tempList[0]);
    if (tempList.size() > 2)
    {
        no_swc_name = TOSTR(tempList[1]) + '-' + TOSTR(tempList[2]);
        pack_name = TOSTR(tempList[1]);
    }
    else if (tempList.size() == 2)
    {
        no_swc_name = TOSTR(tempList[1]);
        pack_name = TOSTR(tempList[1]);
    }
    else if (tempList.size() == 1)
    {
        no_swc_name = TOSTR(tempList[0]);
        pack_name = TOSTR(tempList[0]);
    }

    QString nameFlag = TOQSTR(pack_name);
    if (nameFlag.contains("."))
    {
        nameFlag = nameFlag.split(".").back();
    }
    google::protobuf::Message *msg = proto_pool_->GetProtoMessage(no_swc_name, data);
    if (msg == nullptr)
        return;
    QMap<QString, double> signalMap = toSWCData(*msg, TOQSTR(topicName));
    QMap<QString, double> signalMap_;
    for (auto iter = signalMap.begin(); iter != signalMap.end(); iter++)
    {
        if (needSignal.contains(iter.key()))
            signalMap_[iter.key()] = iter.value();
    }
    emit dataUpdate(timeStamp, signalMap_);
}

QMap<QString, double> ReveiveParserData::toSWCData(const google::protobuf::Message &msg, const QString &topicName)
{
    QMap<QString, double> swc_map;

    auto reflection = msg.GetReflection();
    auto descriptor = msg.GetDescriptor();
    int field_count = descriptor->field_count();

    for (int i = 0; i < field_count; ++i)
    {
        auto field = descriptor->field(i);
        if (field)
        {
            if (field->cpp_type() != 10 && !field->is_repeated()) //判断是否是meesage
            {
                QString val_name = topicName + '~' + TOQSTR(field->camelcase_name());
                swc_map[val_name] = stringToNum<double>(FieldTypeConvert(msg, field));
            }
            else if (field->cpp_type() == 10 && !field->is_repeated())
            {
                const auto &nmsg = reflection->GetMessage(msg, field);
                swc_map.unite(toSWCData(nmsg, topicName + '~' + TOQSTR(field->camelcase_name())));
            }
            else if (field->is_repeated() && field->cpp_type() != 10)
            {
                QString val_name = topicName + '~' + TOQSTR(field->camelcase_name());
                QString value;

                int val_size = msg.GetReflection()->FieldSize(msg, field);
                for (int i = 0; i < val_size; i++)
                {
                    value = getRepeatedMsg(msg, field, i);
                    if (value.isEmpty())
                        continue;

                    swc_map[val_name + "~" + QString::number(i)] = value.toDouble();
                }
            }
            else if (field->is_repeated() && field->cpp_type() == 10)
            {
                int repeatedSize = reflection->FieldSize(msg, field);
                for (int j = 0; j < repeatedSize; ++j)
                {
                    const auto &nmsg = reflection->GetRepeatedMessage(msg, field, j);
                    QString msgName = topicName + '~' + TOQSTR(field->camelcase_name()) + '~' + QString::number(j);
                    swc_map.unite(toSWCData(nmsg, msgName));
                }
            }
        }
    }
    return swc_map;
}

QString ReveiveParserData::getRepeatedMsg(const google::protobuf::Message &msg,
                                          const google::protobuf::FieldDescriptor *field,
                                          int index)
{
    QString res;
    switch (field->type())
    {
    case google::protobuf::FieldDescriptor::TYPE_DOUBLE:
    {
        double value = msg.GetReflection()->GetRepeatedDouble(msg, field, index);
        res = QString::number(value, 'g', 10);
    }
    break;

    case google::protobuf::FieldDescriptor::TYPE_FLOAT:
    {
        float value = msg.GetReflection()->GetRepeatedFloat(msg, field, index);
        res = QString::number(value, 'f', 10);
    }
    break;

    case google::protobuf::FieldDescriptor::TYPE_INT64:
    {
        int64_t value = msg.GetReflection()->GetRepeatedInt64(msg, field, index);
        res = QString::number(value);
    }
    break;

    case google::protobuf::FieldDescriptor::TYPE_UINT64:
    {
        uint64_t value = msg.GetReflection()->GetRepeatedUInt64(msg, field, index);
        res = QString::number(value);
    }
    break;

    case google::protobuf::FieldDescriptor::TYPE_INT32:
    {
        int value = msg.GetReflection()->GetRepeatedInt32(msg, field, index);
        res = QString::number(value);
    }
    break;

    case google::protobuf::FieldDescriptor::TYPE_UINT32:
    {
        uint value = msg.GetReflection()->GetRepeatedUInt32(msg, field, index);
        res = QString::number(value);
    }
    break;

    case google::protobuf::FieldDescriptor::TYPE_BOOL:
    {
        bool value = msg.GetReflection()->GetRepeatedBool(msg, field, index);
        res = QString::number(value);
    }
    break;

    case google::protobuf::FieldDescriptor::TYPE_STRING:
    {
        std::string value = msg.GetReflection()->GetRepeatedString(msg, field, index);
        res = QString::fromStdString(value);
    }
    break;

    case google::protobuf::FieldDescriptor::TYPE_GROUP:
    case google::protobuf::FieldDescriptor::TYPE_MESSAGE:
    case google::protobuf::FieldDescriptor::TYPE_BYTES:
    case google::protobuf::FieldDescriptor::TYPE_ENUM:
    case google::protobuf::FieldDescriptor::TYPE_SFIXED32:
    case google::protobuf::FieldDescriptor::TYPE_SFIXED64:
    case google::protobuf::FieldDescriptor::TYPE_SINT32:
    case google::protobuf::FieldDescriptor::TYPE_FIXED64:
    case google::protobuf::FieldDescriptor::TYPE_FIXED32:
    case google::protobuf::FieldDescriptor::TYPE_SINT64:
        break;
    default:
        break;
    }

    return res;
}