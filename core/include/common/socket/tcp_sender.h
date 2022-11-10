#ifndef TCPSENDER_H
#define TCPSENDER_H

#include <thread>
#include <string>
#include <iostream>
#ifdef __linux__
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <errno.h>
#endif
#include <stdexcept>
using namespace std;

class TcpSender
{
public:
    TcpSender(const int listenPort);
    int send(const void* x,size_t size);
private:
    int fd;
    int fdAccept;
    sockaddr_in receiverAddr;
    sockaddr_in senderAddr;
};
#endif//TCPSENDER_H