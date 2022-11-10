#include "tcp_sender.h"

TcpSender::TcpSender(const int listenPort)
{
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
        throw runtime_error("create socket error");

    receiverAddr.sin_family = AF_INET;
    receiverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    receiverAddr.sin_port = htons(listenPort);
    if (bind(fd, (sockaddr *)&receiverAddr, sizeof(receiverAddr)) == -1)
    {
        throw runtime_error(" bind error");
    }
        
    if (listen(fd, 5) == -1)
    {
        throw runtime_error("listen error");
    } 
}

//haven't figured out how to send data over TCP yet
int TcpSender::send(const void *x, size_t size)
{
    socklen_t addrlen = sizeof(senderAddr);

    fdAccept = accept(fd, (sockaddr *)&senderAddr, &addrlen);
    if (fdAccept == -1)
    {
        throw runtime_error("accept error");
    }
    
    return write(fdAccept, x, size);
}
