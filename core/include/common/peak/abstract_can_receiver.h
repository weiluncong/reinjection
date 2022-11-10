/**
 * @file Abstract_can_receiver.h
 * @author wang kuan (wangkuan@changan.com.cn)
 * @brief the abstract base class to receive can type message
 * @version 0.1
 * @date 2020-09-29
 * 
 * @copyright Copyright (c) 2020 Changan Automobile Co., Ltd. All rights reserved.
 * 
 */

#ifndef ABSTRACT_CAN_RECEIVER_H
#define ABSTRACT_CAN_RECEIVER_H

#include "candata_packer.h"
#include "ktime.h"
#include <thread>


class AbstractCanReceiver
{
public:
    AbstractCanReceiver(int port, int baud, const CandataPacker &packer)
        : port_(port), baud_(baud), packer_(packer)
    {
        minId_ = packer.getMinId();
        maxId_ = packer.getMaxId();
    }
    virtual ~AbstractCanReceiver() {}
    CanDataPack receive();

protected:
    virtual std::vector<CanData> receiveFromDeviceBuffer() = 0;

    unsigned int calculateAccCode(int beginId) const;
    unsigned int calculateAccMask(int beginId, int endId) const;

protected:
    int port_ = 0;
    int baud_ = 0;
    int minId_ = 0;      ///< min id of the receiving range
    int maxId_ = 0xffff; ///< max id of the receiving range

private:
    CandataPacker packer_;
};


#endif //ABSTRACT_CAN_RECEIVER_H