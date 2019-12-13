#include "user.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>

#include <iostream>

#include "src/user/admin.h"
#include "src/user/normal_user.h"

#define IP "127.0.0.1"
#define PORT 9977
#define MESSAGE_LEN 1024

using std::cin;
using std::cout;
using std::endl;

namespace wechat {
User* User::CreateUser(std::string type, std::string name) {
    User* user = nullptr;
    if (type == "1") {
        user = new NormalUser(name);
    } else if (type == "2") {
        user = new Admin(name);
    } else {
        cout << "输入type错误，请重新尝试。" << endl;
    }

    if (user) {
        user->SayHello();
        user->WhatCanIDo();
    }
    return user;
}

void User::Chat() {
    // TODO(hellozmz): impl

    cout << "client is running..." << endl;

    // socket
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr(IP);

    // connect
    int saddr_len = sizeof(serv_addr);
    if (connect(fd, (struct sockaddr *) &serv_addr, saddr_len) < 0) {
        perror("connect error.");
        exit(1);
    }

    // recv && send
    fd_set rfds;
    int maxfd = 0;
    struct timeval tv;
    while(true) {
        FD_ZERO(&rfds);
        FD_SET(0, &rfds);
        maxfd = 0;
        FD_SET(fd, &rfds);
        if(maxfd < fd) {
            maxfd = fd;
        }

        tv.tv_sec = 5;
        tv.tv_usec = 0;
        int rtn = select(maxfd+1, &rfds, NULL, NULL, &tv);
        if (rtn == -1) {
            cout << "select error." << endl;
            break;
        } else if (rtn == 0) {
            // OK
            // cout << "select OK." << endl;
            continue;
        } else {
            // recv, 获取socket标记
            if (FD_ISSET(fd, &rfds)) {
                char buf[MESSAGE_LEN];
                int len = recv(fd, buf, MESSAGE_LEN, 0);
                // cout << "recv data len=" << strlen(buf) << ", clientid=" << fd << ", message=" << buf << endl;
                cout << buf;
                memset(buf, 0, MESSAGE_LEN);
            }
            // send, 删除socket标记
            if (FD_ISSET(0, &rfds)) {
                char buf[MESSAGE_LEN];
                // cin >> buf;
                fgets(buf, MESSAGE_LEN, stdin);
                send(fd, buf, strlen(buf), 0);
                memset(buf, 0, MESSAGE_LEN);
            }
        }
        // sleep(1);
    }
    close(fd);
}

void User::SayHello() {
    cout << "欢迎登陆聊天室。" << endl;
}

} // namespace wechat
