#include "cproto_pool.h"

CProtoPool *CProtoPool::proto_pool_ = GetCProtoPool();

//第一次 映射，读取自带的proto文件。承接capilot发来的proto
void CProtoPool::LoadContentProto()
{
    google::protobuf::compiler::DiskSourceTree source_tree;
    source_tree.MapPath("", "./dbc/proto/idraw/");
    importer_ = new google::protobuf::compiler::Importer(&source_tree, NULL);
    auto file_des = importer_->Import("IdrawContent.proto");
    if (file_des == NULL)
    {
        std::cout << "import file IdrawContent.proto error" << std::endl;
    }
}
//第二次 映射，将框架proto加入impoter中
void CProtoPool::LoadProtoContent(const std::string &content)
{
    LoadContentProto();
    std::ofstream ofs;
    int res = access("./dbc/proto/", 0);
    if (res == -1)
    {
        int r = mkdir("./dbc/proto/", 0777);
    }
    google::protobuf::Message *msg = GetProtoMessage("IdrawContent.ProtoFileContent", content);
    if (msg == nullptr)
        return;
    auto reflection = msg->GetReflection();
    auto descriptor = msg->GetDescriptor();
    auto field = descriptor->field(0);
    int protoFileSize = reflection->FieldSize(*msg, field);
    std::vector<std::string> pcontent;
    for (int protoIndex = 0; protoIndex < protoFileSize; ++protoIndex)
    {
        const auto &nmsg = reflection->GetRepeatedMessage(*msg, field, protoIndex);
        auto content_reflection = nmsg.GetReflection();
        auto content_descriptor = nmsg.GetDescriptor();

        auto content_name_field = content_descriptor->field(0);
        auto content_msg_field = content_descriptor->field(1);
        std::string file_name = content_reflection->GetString(nmsg, content_name_field);
        pcontent.push_back(file_name);
        std::string code = content_reflection->GetString(nmsg, content_msg_field);
        ofs.clear();
        ofs.open("./dbc/proto/" + file_name);
        if (!ofs.is_open())
        {
            std::cout << "open file " << file_name << " error" << std::endl;
        }
        ofs << code;
        ofs.close();
    }
    google::protobuf::compiler::DiskSourceTree source_tree;
    source_tree.MapPath("", "./dbc/proto/");
    importer_ = new google::protobuf::compiler::Importer(&source_tree, NULL);
    for (auto i : pcontent)
    {
        auto file_des = importer_->Import(i);
        if (file_des == NULL)
        {
            std::cout << "import file " << i << " error" << std::endl;
        }
    }
}

google::protobuf::Message *CProtoPool::GetProtoMessage(const std::string &topic_name, const std::string &data)
{
    std::string message_name;
    int pos;
    if (interfaceVersion_ == 3)
    {
        pos = topic_name.find("-");
    }
    else if (interfaceVersion_ == 2)
    {
        pos = topic_name.find("_");
    }

    if (pos != topic_name.npos)
    {
        message_name = topic_name.substr(0, pos);
    }
    else
    {
        message_name = topic_name;
    }
    auto it = msgs_.find(message_name);
    if (it == msgs_.end())
    {
        msgs_[message_name] = CreateMessage(message_name);
    }
    google::protobuf::Message *msg = msgs_[message_name];
    if (msg != nullptr)
    {
        msg->Clear();
        msg->ParseFromString(data);
    }
    return msg;
}

google::protobuf::Message *CProtoPool::CreateMessage(const std::string &message_name)
{
    //    std::cout << "msg name: " << message_name << std::endl;
    auto descriptor = google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(message_name);
    if (descriptor)
    {
        const google::protobuf::Message *protoType = google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
        if (protoType)
        {
            // std::cout << "find message " << messageName << std::endl;
            return protoType->New();
        }
        else
        {
            // throw std::runtime_error("create message failed " + message_name);
            qDebug() << "create message failed " << QString::fromStdString(message_name);
        }
    }
    else
    {
        if (proto_pool_->importer_ != nullptr)
        {
            descriptor = proto_pool_->importer_->pool()->FindMessageTypeByName(message_name);
            if (descriptor)
            {
                return factory_.GetPrototype(descriptor)->New();
            }
        }
        // throw std::runtime_error("can't create proto msg " + message_name);
        qDebug() << "can't create proto msg " << QString::fromStdString(message_name);
    }
    return nullptr;
}

void CProtoPool::updataProtoContent(std::string serviceAddress)
{
    zmq::context_t context;
    zmq::socket_t protoSocket(context, zmq::socket_type::req);
    protoSocket.connect(serviceAddress);
    protoSocket.send(zmq::message_t("hello"), zmq::send_flags::none);
    zmq::message_t msg;
    protoSocket.recv(msg, zmq::recv_flags::none);
    protoContent_ += msg.to_string();
}
