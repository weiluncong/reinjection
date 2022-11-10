#ifndef UDPSENDER_H
#define UDPSENDER_H
#include <stddef.h>
#ifdef __linux
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <string>
#endif
#ifdef WIN32
	#include <WinSock2.h>
    #pragma comment(lib,"WS2_32.lib")
#endif // WIN32

class UdpSender
{
public:
    UdpSender(const char* ip,const int port);
    ~UdpSender();
    int send(const void* x,size_t size);
    int send(const std::string &);
    void printInfo();

private:
    sockaddr_in receiverAddress;
#if __linux
	int socketFd;
#endif // __linux
#ifdef WIN32
	SOCKET socketFd;
	WSADATA wsaData;
#endif // WIN32
};

#endif
