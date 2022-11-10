#include "peak_base.h"
#ifndef qnx
#include <sstream>
#include <iostream>
#include <cstring>
#include "ktime.h"
#include <thread>
using namespace std;

const int kCanmsgByte = 8;
const int kCanfdmsgByte = 64;
int num_ls = 0;
int num_lxs = 0;

const std::map<int, int> kPredefineBaudrate{
    {1000, PCAN_BAUD_1M},
    {800, PCAN_BAUD_800K},
    {500, PCAN_BAUD_500K},
    {250, PCAN_BAUD_250K},
    {125, PCAN_BAUD_125K},
    {100, PCAN_BAUD_100K},
    {95, PCAN_BAUD_95K},
    {83, PCAN_BAUD_83K},
    {50, PCAN_BAUD_50K},
    {47, PCAN_BAUD_47K},
    {33, PCAN_BAUD_33K},
    {20, PCAN_BAUD_20K},
    {10, PCAN_BAUD_10K},
    {5, PCAN_BAUD_5K}};

/**
 * @brief assert devecive status is ok
 * 
 * @param status 
 * @param port port of device
 */
void assertInitializaStatus(TPCANStatus status, int port)
{
    if (status != PCAN_ERROR_OK)
    {
        std::ostringstream os;
        os << "open peak CAN device " << port << " failed!";
        throw std::runtime_error(os.str());
    }
}

/**
 * @brief init the device
 * 
 * @param port port of device, start counting from 1
 * @param baud baud rate for can or data rate for canfd
 * @param isCanFd is init the device for canfd
 * @param filter_min min can message id want to receive
 * @param filter_max max can message id want to receive
 * @return int handle of a PCAN channel
 */
int initPeakDevice(int port, int baud, bool isCanFd, int filter_min, int filter_max)
{
    int channel = 0;
    if (isCanFd)
    {
        channel = openDeviceCanFd(port, baud);
    }
    else
    {
        channel = openDeviceCan(port, baud);
    }

    if (filter_min != -1 && filter_max != -1)
    {
        int res = setMessageFilter(channel, filter_min, filter_max);
        if (res != PCAN_ERROR_OK)
        {
            std::cout << "set CAN device " << port << " msg filter failed!\033[0m" << std::endl;
        }
    }

    return channel;
}

/**
 * @brief release the channel of device
 * 
 * @param channel index of the channel want to release
 */
void releasePeak(int channel)
{
    CAN_Uninitialize(channel);
}

/**
 * @brief open one channel of the device for can protocol
 * 
 * @param port index of channel want to open
 * @param baud baud rate
 * @return int handle of the opened channel
 */
int openDeviceCan(int port, int baud)
{
    unsigned int channel = getChannel(port);
    TPCANStatus status;

    if (kPredefineBaudrate.find(baud) == kPredefineBaudrate.end())
    {
        throw std::runtime_error("input baudrate error!");
    }
    baud = kPredefineBaudrate.at(baud);

    status = CAN_Initialize(channel, baud);

    assertInitializaStatus(status, port);

    return channel;
}

/**
 * @brief open one channel of the device for canfd protocol
 * 
 * @param port index of channel want to open
 * @param baud data rate
 * @return int handle of the opened channel
 */
int openDeviceCanFd(int port, int baud)
{
    unsigned int channel = getChannel(port);
    TPCANStatus status;
    if (baud == 2000) //for now only support 2M CAN_FD
    {
        //sample point 80% 70%
        TPCANBitrateFD bitrate = (char *)"f_clock_mhz=80,nom_brp=2,nom_tseg1=63,nom_tseg2=16,nom_sjw=16,data_brp=1,data_tseg1=27,data_tseg2=12,data_sjw=4";
        // TPCANBitrateFD bitrate = (char *)"f_clock_mhz=80,nom_brp=2,nom_tseg1=63,nom_tseg2=16,nom_sjw=16,data_brp=1,data_tseg1=27,data_tseg2=12,data_sjw=12";
        // TPCANBitrateFD bitrate = (char *)"f_clock_mhz=80,nom_brp=2,nom_tseg1=55,nom_tseg2=24,nom_sjw=2,data_brp=1,data_tseg1=29,data_tseg2=10,data_sjw=10";
        status = CAN_InitializeFD(channel, bitrate);
    }
    else if (baud == 5000)
    {
        //sample point 80% 70%
        // TPCANBitrateFD bitrate = (char *)"f_clock_mhz=80,nom_brp=2,nom_tseg1=63,nom_tseg2=16,nom_sjw=16,data_brp=1,data_tseg1=12,data_tseg2=3,data_sjw=3";
        TPCANBitrateFD bitrate = (char *)"f_clock_mhz=80,nom_brp=2,nom_tseg1=63,nom_tseg2=16,nom_sjw=16,data_brp=1,data_tseg1=11,data_tseg2=4,data_sjw=4";
        // TPCANBitrateFD bitrate = (char *)"f_clock_mhz=40,nom_brp=10,nom_tseg1=2,nom_tseg2=1,nom_sjw=1,data_brp=2,data_tseg1=2,data_tseg2=1,data_sjw=1";
        status = CAN_InitializeFD(channel, bitrate);
    }
    else
    {
        throw std::runtime_error("Baud rate other than 2M are not supported now!");
    }

    assertInitializaStatus(status, port);

    return channel;
}

/** 获取物理端口对应通道编码 */

/**
 * @brief Get the Channel index corresponding physical device port
 * 
 * @param port port of the device, counting from 1
 * @return index of channel
 */
int getChannel(int port)
{
    int fd = 0;
    fd = PCAN_USBBUS1;
    if (port < 1 || port > 16)
    {
        throw std::runtime_error("for PeakSytem 1<= deviceIndex <=16! here is " + std::to_string(port));
    }
    if (port < 9)
    {
        fd += (port - 1); //PCAN-USB interface,channel 1--PCAN_USBBUS1--0x51
                          //PCAN-USB interface,channel 8--PCAN_USBBUS8--0x58
    }
    else
    {
        fd = PCAN_USBBUS9;
        fd += (port - 9); //PCAN-USB interface,channel 9--PCAN_USBBUS9--0x509
                          //PCAN-USB interface,channel 16--PCAN_USBBUS16--0x510
    }

    return fd;
}

/**
 * @brief Set the Message Filter
 * 
 * @param channel index of channel
 * @param min min can message id want to receive
 * @param max max can message id want to receive
 * @return int 0-ok,-1-failed
 */
int setMessageFilter(int channel, int min, int max)
{
    int res = 0;
    TPCANStatus status = CAN_FilterMessages(channel, min, max, PCAN_MODE_STANDARD);
    if (status != PCAN_ERROR_OK)
    {
        res = -1;
    }
    return res;
}

/**
 * @brief read can message from device buffer
 * 
 * @param channel index of channel to receive message
 * @return std::map<int,CanData>
 */
std::vector<CanData> readCanmsg(int channel)
{
    TPCANMsg msg;
    std::vector<TPCANMsg> msgs;
    while (1)
    {
        TPCANStatus status;

        status = CAN_Read(channel, &msg, NULL);

        if (status == PCAN_ERROR_OK) // PCAN_ERROR_OK is returned on success.
        {
            msgs.push_back(msg);
        }
        else if (status == PCAN_ERROR_QRCVEMPTY) //Indicates that the receive queue of the Channel is empty
        {
            break;
        }
        else
        {
            std::cout << "receive met error " << status << std::endl;
            break;
        }
    }

    std::vector<CanData> res;
    for (auto i = msgs.rbegin(); i != msgs.rend(); ++i)
    {
        CanData data;

        data.id = i->ID;
        data.len = 8;
        memcpy(data.data, i->DATA, kCanmsgByte);

        res.push_back(data);
    }

    return res;
}

/**
 * @brief read canfd message form device buffer
 * 
 * @param channel index of channel to receive message
 * @return std::map<int,CanFdData> 
 */
std::vector<CanData> readCanmsgFD(int channel)
{
    TPCANMsgFD msg;
    std::vector<TPCANMsgFD> msgs;
    while (1)
    {
        TPCANStatus status;
        status = CAN_ReadFD(channel, &msg, NULL);

        if (status == PCAN_ERROR_OK) // PCAN_ERROR_OK is returned on success.
        {
            msgs.push_back(msg);
        }
        else if (status == PCAN_ERROR_QRCVEMPTY) //Indicates that the receive queue of the Channel is empty
        {
            break;
        }
        else
        {
            std::cout << "\033[0mreceive met error\033[0m" << status << std::endl;
            break;
        }
    }

    std::vector<CanData> res;
    for (auto i = msgs.rbegin(); i != msgs.rend(); ++i)
    {
        CanData data;

        data.id = i->ID;
        data.len = 64;
        memcpy(data.data, i->DATA, kCanfdmsgByte);

        res.push_back(data);
    }

    return res;
}

/**
 * @brief send canmsg out from specified channel
 * 
 * @param channel specified channel to send message out
 * @param data 
 * @return int the number of can messages sent successfully
 */
int sendCanmsg(int channel, const CanData &data)
{
    int n = 0;

    TPCANMsg msg;
    msg.ID = data.id;
    msg.LEN = 8;
    msg.MSGTYPE = PCAN_MESSAGE_STANDARD;

    memcpy(msg.DATA, data.data, kCanmsgByte);

    TPCANStatus status = CAN_Write(channel, &msg);
    if (status != PCAN_ERROR_OK)
    {
        char info[50];
        CAN_GetErrorText(status, 0, info);
        std::cout << info << std::endl;
    }
    if (PCAN_ERROR_BUSOFF == status || PCAN_ERROR_QXMTFULL == status)
    {
        CAN_Reset(channel);
    }
    n++;
    return n;
}

/**
 * @brief send canfd message out from specified channel
 * 
 * @param channel specified channel to send message out
 * @param data 
 * @param dlc 
 * @return int the number of can messages sent successfully
 */
int sendCanmsgFD(int channel, const CanData &data)
{
    int n = 0;
    // std::cout << n << std::endl;
    TPCANMsgFD msg;
    msg.ID = data.id;
    if (data.len == 0)
    {
        std::cout << data.id << " ==== " << channel << "----\n";
    }
    msg.DLC = canDataLenToDlc(data.len);
    msg.MSGTYPE = PCAN_MESSAGE_STANDARD | PCAN_MESSAGE_FD | PCAN_MESSAGE_BRS;
    // msg.MSGTYPE = PCAN_MESSAGE_STANDARD | PCAN_MESSAGE_FD;
    // msg.MSGTYPE = PCAN_MESSAGE_FD | PCAN_MESSAGE_BRS;

    memcpy(msg.DATA, data.data, kCanfdmsgByte);
    TPCANStatus status = CAN_WriteFD(channel, &msg);
    if (status == PCAN_ERROR_OK)
    {
        num_lxs++;
    }
    if (status != PCAN_ERROR_OK)
    {
        char info[50];
        CAN_GetErrorText(status, 0, info);
        std::cout << info << std::endl;
    }
    if (PCAN_ERROR_BUSOFF == status || PCAN_ERROR_QXMTFULL == status)
    {
        CAN_Reset(channel);
    }

    n++;
    return n;
}

#endif //qnx
