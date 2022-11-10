#include "udp_sender.h"
#include <cstring>
#include <iostream>

using namespace std;

UdpSender::UdpSender(const char* ip,const int port)
{
#ifdef WIN32
	WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif // WIN32
    receiverAddress.sin_family=AF_INET;
    receiverAddress.sin_addr.s_addr=inet_addr(ip);
    receiverAddress.sin_port=htons(port);

#ifdef __linux
	socketFd = socket(AF_INET, SOCK_DGRAM, 0);
	if (socketFd == -1)
		cout << " failed to create sender socket" << endl;
#endif // __linux

#ifdef WIN32
	socketFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (socketFd == SOCKET_ERROR)
		cout << "failed to create receiver socket" << endl;
#endif // WIN32

   
    

}

UdpSender::~UdpSender()
{
#ifdef __linux
	close(socketFd);
#endif // __linux

#ifdef WIN32
	closesocket(socketFd);
	WSACleanup();
#endif // WIN32
}

void UdpSender::printInfo()
{
   // cout<<"send to: "<<inet_ntoa(receiverAddress.sin_addr)<<"  port: "<<ntohs(receiverAddress.sin_port)<<endl;
}


int UdpSender::send(const void* x,size_t size)
{
    int res=sendto(socketFd,(char*)x,size,0,(sockaddr*)&receiverAddress,sizeof(receiverAddress));
    if(res==-1)
        cout<<"send failed"<<endl;
    return res;
}

int UdpSender::send(const std::string & data){
	std::string tmp = data;
	return send(&tmp[0], tmp.length());
}