#include "socket_unix_sender.h"
#include <cstring>
#include <iostream>

using namespace std;

SocketUnixSender::SocketUnixSender(const std::string &path)
{
#ifdef _WIN32
	throw std::runtime_error("SocketUnixSender only work under linux");
#else
	receiverAddress.sun_family=AF_LOCAL;
	strcpy(receiverAddress.sun_path,path.c_str());

	socketFd = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (socketFd == -1)
	{
		std::cout<<" failed to create sender socket"<<std::endl;
	}
#endif
}

SocketUnixSender::~SocketUnixSender()
{
#ifdef _WIN32
	closesocket(socketFd);
	WSACleanup();
#else
	close(socketFd);
#endif // _WIN32
}


int SocketUnixSender::send(const void* x,size_t size)
{
#ifdef _WIN32
	return -1;
#else
    int res=sendto(socketFd,(char*)x,size,0,(sockaddr*)&receiverAddress,sizeof(receiverAddress));
    if(res==-1)
        cout<<"socket send failed"<<endl;
    return res;
#endif
}


