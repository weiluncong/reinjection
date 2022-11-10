/**
 * @file peak_can_sender.h
 * @author wang kuan (wangkuan@changan.com.cn)
 * @brief send can or canfd data by peak system
 * @version 0.1
 * @date 2020-09-29
 * 
 * @copyright Copyright (c) 2020 Changan Automobile Co., Ltd. All rights reserved.
 * 
 */

#ifndef PEAK_CAN_SENDER_H
#define PEAK_CAN_SENDER_H
#ifndef qnx

#include "abstract_can_sender.h"
#include "peak_base.h"

class PeakCanSender : public AbstractCanSender
{
public:
    PeakCanSender(int port, int baud)
        : AbstractCanSender()
    {
        pcan_device_ = initPeakDevice(port, baud, false);
    }
    ~PeakCanSender()
    {
        releasePeak(pcan_device_);
    }
    virtual int send(const CanData &data) override
    {
        return sendCanmsg(pcan_device_, data);
    }

private:
    int pcan_device_ = -1;
};


class PeakCanFdSender : public AbstractCanSender
{
public:
    PeakCanFdSender(int port, int baud)
        : AbstractCanSender()
    {
        pcan_device_ = initPeakDevice(port, baud, true);
    }
    ~PeakCanFdSender()
    {
        releasePeak(pcan_device_);
    }
    virtual int send(const CanData &data) override
    {
        return sendCanmsgFD(pcan_device_, data);
    }

private:
    int pcan_device_ = -1;
};

#endif //qnx
#endif //PEAK_CAN_SENDER_H
