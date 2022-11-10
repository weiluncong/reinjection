import socket
class udpSender:
    # 1.创建一个udp套接字
    udp_socket = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
    # def __init__(self):
        # self.udp_socket 

    def send(self,data):
        self.udp_socket.sendto(data.encode("utf-8"),("127.0.0.1",30000)) # 2.准备接收方的地址

    def close(self):
        self.udp_socket.close()

# def main():
#     sender = udpSender()
#     sender.send("hello world")
#     sender.close()

# if __name__ == "__main__":
#     main()
