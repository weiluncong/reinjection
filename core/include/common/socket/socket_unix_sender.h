
#ifndef SOCKET_UNIX_SENDER_H
#define SOCKET_UNIX_SENDER_H

#ifdef _WIN32
	#include <WinSock2.h>
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <sys/un.h>
    #include <unistd.h>
    #include <arpa/inet.h>
#endif // _WIN32
#include <string>

class SocketUnixSender
{
public:

    SocketUnixSender(const std::string &path);
    ~SocketUnixSender();

    /**
     * 执行数据的发送
     * \param x 拟发送数据起始内存地址
     * \param size 拟发送数据大小
     * \warning size大于x实际空间大小或大于64k，程序均会出错
     */
    int send(const void* x,size_t size);

private:
#ifdef _WIN32
	SOCKET socketFd;
	WSADATA wsaData;
#else
    sockaddr_un receiverAddress;
	int socketFd;
#endif // _WIN32
};

#endif