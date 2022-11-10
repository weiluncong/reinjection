#include "candata_packer.h"
#include <iostream>

using namespace std;

/**
 * @brief add a msg packer
 * 
 * @param packagedId all the id want to pack
 * @param msgSource indicate the source of can data
 * @param model pack when the data contains all the specified id or when the specified packTime is up 
 * @param packTime if the model is PACK_WHEN_TIMES_UP, this param will work
 */
void CandataPacker::addPacker(const std::vector<int> &packagedId, int msgSource, int dataType, PackModel model, int packTime)
{
    TDataPack pack;
    pack.flag = msgSource;
    pack.model = model;
    pack.packTime = packTime;
    pack.type = dataType;

    for (auto i : packagedId)
    {
        pack.receiveId.insert(i);
        updateReceiveRange(i);
    }

    data_.push_back(pack);
}

void CandataPacker::addData(const std::vector<CanData> &data, double timestamp)
{
    for (auto i : data)
    {
        // std::cout << " i : - " << i.id << std::endl;
        addData(i, timestamp);
        // if(flag_ == 0)
        // {
        //     flag_ = i.id;
        //     ids_.push_back(i.id);
        // }
        // if(flag_ == i.id && flag_ != -1 && ids_.size() > 190)
        // {std::cout << " ------------------"  << std::endl;
        //     for(auto i : ids_)
        //     {
        //         std::cout << "0x" << i << ","<< std::endl;
        //     }
        //    std::cout << " ------------------"  << std::endl;
           
        //    flag_ = -1;
        // }
        // else if (flag_ != -1){
        //     ids_.push_back(i.id);
        // }
    }
}

void CandataPacker::setCacheClearTimeoutThreshold(double millisecond)
{
    timeoutThres_ = millisecond / 1000.0;
}

void CandataPacker::addData(const CanData &data, double timestamp)
{
    int id = data.id;
    for (auto &i : data_)
    {
        if (i.receiveId.find(id) == i.receiveId.end())
        {
            continue;
        }

        bool isCanReturn = false;
        switch (i.model)
        {
        case PackModel::PACK_WHEN_GET_ALL_MSG:
            /**
                 * if the current data's timestamp is more than 1ms than the previous data,
                 * all previous data will be cleared
                 */
            if (i.timestamp != 0 && timestamp - i.timestamp > timeoutThres_)
            {
                // cout<<"delay-------------------------------:  "<<std::fixed<<timestamp - i.timestamp<<"   id:   "<<id << endl;
                i.data.clear();
            }

            i.data[id] = data;       //add data
            i.timestamp = timestamp; //update time stamp
            isCanReturn = i.data.size() == i.receiveId.size();

            break;
        case PackModel::PACK_WHEN_TIMES_UP:
        case PackModel::PACK_SAMPLE:
            i.data[id] = data;    //add data
            if (i.timestamp == 0) //init time stamp
            {
                i.timestamp = timestamp;
            }
            else
            {
                isCanReturn = (timestamp - i.timestamp) * 1000 > i.packTime; //packTime's unit is ms, timestamp's unit is s
            }
            break;
        default:
            break;
        }

        if (isCanReturn)
        {
            toReturn_.push(i);
            if (i.model == PackModel::PACK_SAMPLE)
            {
                i.timestamp = timestamp;
            }
            else
            {
                i.data.clear();
                i.timestamp = 0;
            }
        }
    }
}

CanDataPack CandataPacker::getDataPack()
{
    CanDataPack res;
    if (!toReturn_.empty())
    {
        const TDataPack &pack = toReturn_.front();
        res.datamap = pack.data;
        res.flag = pack.flag;
        res.type = pack.type;
        res.timestamp = pack.timestamp;
        

        toReturn_.pop();
    }

    return res;
}

void CandataPacker::updateReceiveRange(int id)
{
    minId_ = minId_ > id ? id : minId_;
    maxId_ = maxId_ > id ? maxId_ : id;
}