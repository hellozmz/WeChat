/**
 * Copyright
*/

/*局域网TCP客户端*/
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <istream>

#define MYPORT  6845
#define BUFFER_SIZE 1024

/*在TCP三次握手完成后会进入等待连接队列，等待服务端调用accpet与之建立连接，这时候是server端调用accept跟客户端建立
通信，客户端并不需要调用accpet，因为有很多个客户端要跟服务端建立连接，这时候服务端就会有一个队列，对已经经过三次握
手的才可以建立连接（类似缓存信息），这个是由服务端来确认的，客户端并不知道什么时候服务端才能跟它建立连接，在服务端
没有调用accept与之连接或者还未排队到它，只能是一直等待，直到服务端准备好了才能跟客户端建立连接，所以主动权在服务端*/

int main() {
    // 定义sockfd
    int sock_cli = socket(AF_INET, SOCK_STREAM, 0);

    // 定义sockaddr_in
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(MYPORT);  // 服务器端口
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
                        // 服务器ip，inet_addr用于IPv4的IP转换（十进制转换为二进制）
    // 127.0.0.1是本地预留地址
    // 连接服务器，成功返回0，错误返回-1
    if (connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("connect");
        exit(1);
    }

    char sendbuf[BUFFER_SIZE];
    char recvbuf[BUFFER_SIZE];

    while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL) {
        /*每次读取一行，读取的数据保存在buf指向的字符数组中，成功，则返回第一个参数buf；*/
        send(sock_cli, sendbuf, strlen(sendbuf), 0);     // 发送
        if (strcmp(sendbuf, "exit\n") == 0)
            break;
        recv(sock_cli, recvbuf, sizeof(recvbuf), 0);     // 接收
        fputs(recvbuf, stdout);

        memset(sendbuf, 0, sizeof(sendbuf));    // 接受或者发送完毕后把数组中的数据全部清空（置0）
        memset(recvbuf, 0, sizeof(recvbuf));
    }
    close(sock_cli);
    return 0;
}
