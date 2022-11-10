/**
 * \file udp_receiver.h 
 * \brief 定义udp数据接收的类
 * \author 王宽
 * \date 2020-02-18 
 */

#ifndef UDPRECEIVER_H
#define UDPRECEIVER_H
#include <memory>
#ifdef _WIN32
	#include <WinSock2.h>
#else
    #include <netinet/in.h>
#endif // _WIN32

/**
 * Udp数据接收器
 * \note 该类平台无关
 */
class UdpReceiver
{
public:
    /**
     * 构造函数
     * \param receverPort 本地监听端口
     * \param WAIT 是否设置为阻塞模式，默认是
     */
    UdpReceiver(int receiverPort,bool WAIT=true);
    ~UdpReceiver();
    
    int getPort(){return port;}

    /**
     * 执行数据接收操作
     * \param[out] x 用于存放输出的空间
     * \param size 希望接收的数据大小
     * \return 实际接收到的数据的大小
     */
    int receive(void* const x,size_t size) const;

    /**
     * 打印该udp数据接收器的相关信息
     */
    void printUdpReceiverInformation() const; 
private:
    void initial(int receiverPort);
private:
    sockaddr_in senderAddress;
    sockaddr_in receiverAddress;

#ifdef _WIN32
	SOCKET socketFd;
#else
	int socketFd;
#endif // _WIN32
    
    int port;
    bool isWait;
};

typedef std::shared_ptr<UdpReceiver> UdpReceiverPtr;



#endif