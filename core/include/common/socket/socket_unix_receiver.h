#ifndef SOCKET_UNIX_RECEIVER_H
#define SOCKET_UNIX_RECEIVER_H
#include <memory>
#ifdef _WIN32
#include <WinSock2.h>
#else
#include <sys/un.h>
#endif // _WIN32
#include <string>


class SocketUnixReceiver
{
public:
    /**
     * 构造函数
     * \param receverPort 本地监听端口
     * \param WAIT 是否设置为阻塞模式，默认是
     */
    SocketUnixReceiver(const std::string &path, bool WAIT = true);
    ~SocketUnixReceiver();

    /**
     * 执行数据接收操作
     * \param[out] x 用于存放输出的空间
     * \param size 希望接收的数据大小
     * \return 实际接收到的数据的大小
     */
    int receive(void *const x, size_t size) const;

    /**
     * 打印该udp数据接收器的相关信息
     */
    void printSocketUnixReceiverInformation() const;

private:
    void initial();

private:
#ifdef _WIN32
    SOCKET socketFd;
#else
    sockaddr_un senderAddress_;
    sockaddr_un receiverAddress_;
    int socketFd;
#endif // _WIN32

    std::string path_;
    bool isWait;
};

typedef std::shared_ptr<SocketUnixReceiver> SocketUnixReceiverPtr;

#endif