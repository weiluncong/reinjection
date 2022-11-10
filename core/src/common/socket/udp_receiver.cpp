#include "udp_receiver.h"
#include <iostream>
#ifndef _WIN32
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

UdpReceiver::UdpReceiver(int receiverPort, bool WAIT)
	: port(receiverPort), isWait(WAIT)
{
	initial(receiverPort);
}

UdpReceiver::~UdpReceiver()
{
#ifdef _WIN32
	closesocket(socketFd);
	WSACleanup();
#else
	close(socketFd);
#endif // _WIN32
}

int UdpReceiver::receive(void *const x, size_t size) const
{
#ifdef _WIN32
	int len;
#else
	socklen_t len;
#endif

	len = sizeof(senderAddress);

	int res = -1;

	res = recvfrom(socketFd, (char *)x, size, 0, (sockaddr *)&senderAddress, &len);
	if (res == -1)
	{
		std::cout<<"receive failed!"<<std::endl;;
	}

	return res;
}

void UdpReceiver::printUdpReceiverInformation() const
{
	std::cout << "receiverAddress: " << receiverAddress.sin_addr.s_addr << " receiverPort: " << ntohs(receiverAddress.sin_port) << std::endl;
}

void UdpReceiver::initial(int receiverPort)
{
#ifdef _WIN32
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	socketFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	receiverAddress.sin_family = AF_INET;
	receiverAddress.sin_addr.s_addr = INADDR_ANY;
	receiverAddress.sin_port = htons(receiverPort);

	if (socketFd == INVALID_SOCKET)
	{
		std::cout<<"failed to create receiver socket"<<std::endl;
	}
	int ret = bind(socketFd, (sockaddr *)&receiverAddress, sizeof(receiverAddress));
	if (ret == -1)
	{
		std::cout<<"bind receiver socket failed"<<std::endl;
	}
#else
	receiverAddress.sin_family = AF_INET;
	receiverAddress.sin_addr.s_addr = INADDR_ANY;
	receiverAddress.sin_port = htons(receiverPort);

	socketFd = socket(AF_INET, SOCK_DGRAM, 0);
	if (socketFd == -1)
	{
		std::cout<<"failed to create receiver socket"<<std::endl;;
	}

	int ret = bind(socketFd, (sockaddr *)&receiverAddress, sizeof(receiverAddress));
	if (ret == -1)
	{
		std::cout<<"bind receiver socket failed"<<std::endl;
	}
#endif // _WIN32
}
