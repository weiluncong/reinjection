#include "tcp_receiver.h"

TcpReceiver::TcpReceiver(const char*receiverIp, const int receiverPort)
{
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
    {
        throw runtime_error("create socket error");
    }

    sockaddr_in receiverAddr;
    receiverAddr.sin_family = AF_INET;
    receiverAddr.sin_addr.s_addr = inet_addr(receiverIp);
    receiverAddr.sin_port = htons(receiverPort);
    if (connect(fd, (sockaddr *)&receiverAddr, sizeof(receiverAddr)) == -1)
    {
        string erroInfo = "connect failed";
        cout << errno << endl;
        throw runtime_error(erroInfo);
    }
    cout << "receiver::connect success" << endl;
}

int TcpReceiver::receive(void *x, size_t size)
{
    return read(fd, x, size);
}
