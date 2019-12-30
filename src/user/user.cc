#include "user.h"

#include <boost/algorithm/string.hpp>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>

#include <chrono>
#include <iostream>
#include <thread>

#include "config/config.h"
#include "src/user/admin.h"
#include "src/user/normal_user.h"

#include "glog/logging.h"


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

void User::JoinChatRoom() {
    cout << "欢迎加入聊天室！" << endl;

    // socket
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    int reuse = 0;
#ifdef debug_wechat
    reuse = 1;
#endif
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

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
    LOG(ERROR) << "connect successful.";

    // 第一次连接server后，发送名字
    {
        char tmp_buf[MESSAGE_LEN];
        SendName(tmp_buf, MESSAGE_LEN);
        std::string name(tmp_buf);
        cout << "send name = " << name << endl;
        send(fd, tmp_buf, strlen(tmp_buf), 0);
        memset(tmp_buf, 0, MESSAGE_LEN);
    }

    // recv && send
    fd_set rfds;
    int maxfd = 0;
    struct timeval tv;
    int retry_time = 3;
    std::string close_user(CLOSE_USER);

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

        int rtn = select(maxfd+1, &rfds, NULL, NULL, /*等待时间*/ /*&tv*/ NULL);

        if (rtn < 0) {
            LOG(ERROR) << "select error, error code=" << rtn;
            break;
        } else if (rtn == 0) {
            // 等待超时
            // LOG(ERROR) << "client wait overtime.";
            continue;
        } else {
            // recv, 获取socket标记
            if (FD_ISSET(fd, &rfds)) {
                char buf[MESSAGE_LEN];
                int len = recv(fd, buf, MESSAGE_LEN, 0);
                if (len == 0) {
                    cout << "server端不在线，请检查，5秒后重试，再重试"<< retry_time-- << "次后退出。" << endl;
                    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
                    if (retry_time == 0) {
                        // close(fd);
                        break;
                    }
                }
                cout << buf << std::flush;
                memset(buf, 0, MESSAGE_LEN);
            }

            // send, 并删除socket标记
            if (FD_ISSET(0, &rfds)) {
                char buf[MESSAGE_LEN];
                fgets(buf, MESSAGE_LEN, stdin);
                std::string get_msg(buf);
                if (boost::starts_with(get_msg, close_user)) {
                    break;
                }
                send(fd, buf, strlen(buf), 0);
                memset(buf, 0, MESSAGE_LEN);
            }
        }
    }
    cout << "退出聊天室" << endl;
    close(fd);
}

void User::SayHello() {
    cout << "欢迎登陆聊天室。" << endl;
}

bool User::SendName(char* buffer, int length) {
    std::string send_username = SEND_NAME_PREFIX + GetUserName();
    if (send_username.size() + sizeof(SEND_NAME_PREFIX) >= length) {
        send_username = SEND_NAME_PREFIX;
        send_username += "abc";
        return false;
    }
    for (int i=0; i<send_username.size(); ++i) {
        buffer[i] = send_username[i];
    }
    buffer[send_username.size()] = '\0';
    return true;
}

} // namespace wechat
