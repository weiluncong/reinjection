#ifndef TCPRECEIVER_H
#define TCPRECEIVER_H

#include <string>
#include <iostream>
#ifdef __linux__
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#endif

using namespace std;

class TcpReceiver
{
public:
    TcpReceiver(const char *receiverIp, const int receiverPort);
    ~TcpReceiver() { close(fd); }
    int receive(void *x, size_t size);

private:
    int fd;
};
#endif //TCPRECEIVER_H