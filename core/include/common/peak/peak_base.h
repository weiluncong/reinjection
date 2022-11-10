#ifndef PEAK_BASE_H
#define PEAK_BASE_H

#ifndef qnx
#ifdef _WIN32
#include "PCANBasic_win.h"
#else
#include "PCANBasic.h"
#endif
#include "can_structure.h"
#include <map>

void assertInitializaStatus(int status, int port);
void releasePeak(int channel);
int openDeviceCan(int port, int baud);
int openDeviceCanFd(int port, int baud);
int getChannel(int port);
int setMessageFilter(int channel, int min, int max);

int initPeakDevice(int port, int baud, bool isCanFd, int filter_min = -1, int filter_max = -1);
std::vector<CanData> readCanmsg(int channel);
std::vector<CanData> readCanmsgFD(int channel);
int sendCanmsg(int channel, const CanData &data);
int sendCanmsgFD(int channel, const CanData &data);

#endif //PEAK_BASE_H

#endif //qnx
