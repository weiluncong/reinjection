/**
 * @file peak_can_receiver.h
 * @author wang kuan (wangkuan@changan.com.cn)
 * @brief receive can or canfd data by peak system
 * @version 0.1
 * @date 2020-09-29
 * 
 * @copyright Copyright (c) 2020 Changan Automobile Co., Ltd. All rights reserved.
 * 
 */
#ifndef PEAK_CAN_RECEIVER_H
#define PEAK_CAN_RECEIVER_H

#ifndef qnx
#include "abstract_can_receiver.h"
#include "peak_base.h"
#include <iostream>
#include <iomanip>

class PeakCanReceiver : public AbstractCanReceiver
{
public:
    PeakCanReceiver(int port, int baud, const CandataPacker &packer)
        : AbstractCanReceiver(port, baud, packer)
    {
        pcan_device_ = initPeakDevice(port_, baud_, false, minId_, maxId_);
    }
    ~PeakCanReceiver()
    {
        releasePeak(pcan_device_);
    }

protected:
    virtual std::vector<CanData> receiveFromDeviceBuffer() override
    {
        return readCanmsg(pcan_device_);
    }

private:
    int pcan_device_ = 0;
};

class PeakCanFdReceiver : public AbstractCanReceiver
{
public:
    PeakCanFdReceiver(int port, int baud, const CandataPacker &packer)
        : AbstractCanReceiver(port, baud, packer)
    {
        pcan_device_ = initPeakDevice(port_, baud_, true, minId_, maxId_);
    }
    ~PeakCanFdReceiver()
    {
        releasePeak(pcan_device_);
    }

protected:
    virtual std::vector<CanData> receiveFromDeviceBuffer() override
    {
        auto res = readCanmsgFD(pcan_device_);
        // int n = res.size();
        // if (n != 0)
        // {
        //     for (int i = 0; i < n; ++i)
        //     {
        //         int id=res[i].id;
        //         if(id==0x190||id==0x191||id==0x192||id==0x1a7)
        //         {
        //             cout << hex << res[i].id << " ";
        //         }
        //     }
        //     cout << endl;
        // }
        return res;
    }

private:
    int pcan_device_ = 0;
};

#endif //qnx

#endif //PEAK_CAN_RECEIVER_H
