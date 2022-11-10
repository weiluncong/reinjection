#include "abstract_can_sender.h"
#include <iostream>
#include "ktime.h"
using namespace std;

int AbstractCanSender::send(const CanDataMap& datamap)
{
    std::vector<CanData> data;
    for(auto i:datamap)
    {
        data.push_back(i.second);
    }
    for(auto i : data)
    send(i);
    // cout<<res<<" "<<KTime().getStringTimeMillisecond()<<endl;
    return data.size();
}
