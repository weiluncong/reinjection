#include "modifySignalVal.h"

std::string modifySignalVal(const std::string & MassageName, const std::string &data, const std::string & signalName, int32_t val){
    CProtoPool * cprotoPool_ = CProtoPool::GetCProtoPool();
    google::protobuf::Message *msg = cprotoPool_->GetProtoMessage(MassageName, data);
    auto reflection = msg->GetReflection();

    auto descriptor = msg->GetDescriptor();

    // auto protoType = google::protobuf::MassageFactory::generated_factory()->GetPrototype(descriptor);

    auto field = descriptor->FindFieldByName(signalName);

    reflection->SetUInt32(msg, field, val);

    std::string res;
    msg->SerializeToString(&res);
    return res;
}