import socket
import threading
def main():
    print ("start")
    try:
        threading._start_new_thread(recv,("Thread-1",2,))
    except:
        print ("Error: 无法启动线程")
    while 1:
        pass

def recv(threadName,delay):
    print (threadName,delay)
    udp_socket = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)

    local_addr = ("",30000)
    udp_socket.bind(local_addr)

    while 1:
        recv_data = udp_socket.recvfrom(65536)

        print(recv_data[0].decode('gbk'))
        # print(recv_data[1])

    udp_socket.close()


if __name__ == "__main__":
    main()