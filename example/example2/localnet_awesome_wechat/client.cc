#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>

#define MYPORT  7000
#define BUFFER_SIZE 1024
int main()
{
    int sock_cli;
    fd_set rfds;
    struct timeval tv;
    int retval, maxfd;

    ///定义sockfd
    sock_cli = socket(AF_INET,SOCK_STREAM, 0);
    ///定义sockaddr_in
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(MYPORT);  ///服务器端口
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  ///服务器ip

    //连接服务器，成功返回0，错误返回-1
    if (connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("connect");
        exit(1);
    }

    while(1){
        /*把可读文件描述符的集合清空*/
        FD_ZERO(&rfds);
        /*把标准输入的文件描述符加入到集合中*/
        FD_SET(0, &rfds);
        maxfd = 0;
        /*把当前连接的文件描述符加入到集合中*/
        FD_SET(sock_cli, &rfds);
        /*找出文件描述符集合中最大的文件描述符*/    
        if(maxfd < sock_cli)
            maxfd = sock_cli;
        /*设置超时时间*/
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        /*等待聊天*/
        retval = select(maxfd+1, &rfds, NULL, NULL, &tv);
        if(retval == -1){
            printf("select出错，客户端程序退出\n");
            break;
        }else if(retval == 0){
            // printf("客户端没有任何输入信息，并且服务器也没有信息到来，waiting...\n");
            continue;
        }else{
            /*服务器发来了消息*/
            if(FD_ISSET(sock_cli,&rfds)){
                char recvbuf[BUFFER_SIZE];
                int len;
                len = recv(sock_cli, recvbuf, sizeof(recvbuf),0);
                printf("%s", recvbuf);
                memset(recvbuf, 0, sizeof(recvbuf));
            }
            /*用户输入信息了,开始处理信息并发送*/
            if(FD_ISSET(0, &rfds)){
                char sendbuf[BUFFER_SIZE];
                fgets(sendbuf, sizeof(sendbuf), stdin);
                send(sock_cli, sendbuf, strlen(sendbuf),0); //发送
                memset(sendbuf, 0, sizeof(sendbuf));
            }
        }
    }

    close(sock_cli);
    return 0;
}