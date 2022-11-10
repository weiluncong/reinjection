/**
 * @file candata_packer.h
 * @author wang kuan (wangkuan@changan.com.cn)
 * @brief pack the can messages of specified range togethor and return
 * @version 0.1
 * @date 2020-09-28
 * 
 * @copyright Copyright (c) 2020 Changan Automobile Co., Ltd. All rights reserved.
 * 
 */

#ifndef CANMSG_PACKER_H
#define CANMSG_PACKER_H

#include "can_structure.h"
#include <queue>
#include <vector>
#include <iostream>
#include <set>

using std::cout;
using std::endl;

enum class PackModel
{
    PACK_WHEN_GET_ALL_MSG,
    PACK_WHEN_TIMES_UP,
    PACK_SAMPLE
};

struct TDataPack
{
    int flag = 0;
    int type = 0;
    double timestamp = 0;
    CanDataMap data; //key is msg id
    std::set<int> receiveId;
    PackModel model;
    int packTime = 0;
};

class CandataPacker
{

public:
    CandataPacker() {}
    ~CandataPacker() {}

    void addPacker(const std::vector<int> &packagedId, int msgSource, int dataType, PackModel model, int packTime = 20);

    void addData(const std::vector<CanData> &data, double timestamp);

    void addData(const CanData &data, double timestamp);

    void setCacheClearTimeoutThreshold(double millisecond);
    CanDataPack getDataPack();

    int getMinId() const { return minId_; }
    int getMaxId() const { return maxId_; }

private:
    void updateReceiveRange(int id);

private:
    std::vector<TDataPack> data_;
    std::queue<TDataPack> toReturn_;
    double timeoutThres_ = 25; ///< second
    int minId_ = 0;            ///< min id of the receiving range
    int maxId_ = 0xffff;       ///< max id of the receiving range
    std::vector<int> ids_;
    int flag_ = 0;
};

#endif //CANMSG_PACKER_H
