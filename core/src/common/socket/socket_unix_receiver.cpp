#include "socket_unix_receiver.h"
#include <iostream>
#ifndef _WIN32
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

SocketUnixReceiver::SocketUnixReceiver(const std::string& path, bool WAIT)
	: path_(path), isWait(WAIT)
{
	initial();
}

SocketUnixReceiver::~SocketUnixReceiver()
{
#ifdef _WIN32
	closesocket(socketFd);
	WSACleanup();
#else
	close(socketFd);
#endif // _WIN32
}

int SocketUnixReceiver::receive(void *const x, size_t size) const
{
	int res=-1;
#ifdef _WIN32
	throw std::runtime_error("the SocketUnixReceiver only work under linux");
#else
	socklen_t len;
	len = sizeof(senderAddress_);
	res = recvfrom(socketFd, (char *)x, size, 0, (sockaddr *)&senderAddress_, &len);
	if (res == -1)
	{
		std::cout<<"receive failed!"<<std::endl;;
	}

#endif
	return res;
}


void SocketUnixReceiver::initial()
{
#ifdef _WIN32
#else
	receiverAddress_.sun_family = AF_UNIX;
	strcpy(receiverAddress_.sun_path,path_.c_str());

	socketFd = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (socketFd == -1)
	{
		std::cout<<"failed to create receiver socket"<<std::endl;;
	}
	int nRecvBuf = 10240;//设置为10K

	setsockopt(socketFd, SOL_SOCKET,SO_RCVBUF, (const char*)&nRecvBuf, sizeof(int));

	unlink(receiverAddress_.sun_path);
	int ret = bind(socketFd, (sockaddr *)&receiverAddress_, sizeof(receiverAddress_));
	if (ret == -1)
	{
		std::cout<<"bind receiver socket failed"<<std::endl;
	}
#endif // _WIN32
}
