#include <boost/algorithm/string.hpp>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdarg.h>
// #include <sys/prctl.h>   /*只在Linux下可以使用*/
#include <sys/socket.h>
#include <sys/stat.h>

#include <chrono>
#include <cstring>
#include <iostream>
#include <list>
#include <map>
#include <mutex>
#include <string>
#include <thread>

#include "config/config.h"
#include "util/glog_util.h"

using std::cin;
using std::cout;
using std::endl;

using UserDatabase = std::map<int, std::string>;

std::mutex data_mutex;

struct UserDB {
    int fd;
    std::string user_name;
};

// 监听注册的client socket
void AcceptSocket(int skt, struct sockaddr_in s_addr, socklen_t saddr_len,
                  std::list<int>& socket_list) {
    // prctl(PR_SET_NAME, "WeChatAcceptSocket");
    while(true) {
        int fd = accept(skt, (sockaddr*)&s_addr, &saddr_len);
        cout << fd << " is linked." << endl;
        socket_list.push_back(fd);

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

// IO多路复用
void RecvData(std::list<int>& socket_list, std::string& data) {
    struct timeval timeout;
    while(true) {
        for (auto i : socket_list) {
            fd_set rfds;        // select监听的IO多路复用的数据空间
            FD_ZERO(&rfds);
            int maxfd = 0;      // select监听的文件句柄的上限数
            FD_SET(i, &rfds);
            if(maxfd < i){
                maxfd = i;
            }
            // select调用完后，timeout的值可能被修改，是一个不确定的值，所以cpu可能飙升到100%
            // 所以需要在while循环内部每次都赋值一遍
            timeout.tv_sec = 2;
            timeout.tv_usec = 0;
            int rtn = select(maxfd+1, &rfds, NULL, NULL, &timeout);
            if (rtn < 0) {
                cout << "select error." << endl;
            } else if (rtn == 0) {
                // 连接超时
                // cout << "select OK." << endl;
            } else {
                // recv
                char buf[MESSAGE_LEN];
                memset(buf, 0, MESSAGE_LEN);
                int len = recv(i, buf, MESSAGE_LEN, 0);
                if (len != 0) {         // 没有客户端连接的时候，不要打印数据
                    cout << "recv message len=" << strlen(buf)
                         << ", clientid=" << i << ", message=" << buf << endl;
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

// send 向所有的用户都发送数据
void SendData(std::list<int>& socket_list, std::string& data) {
    while(true) {
        char buf[MESSAGE_LEN];
        cin >> buf;
        for(auto i : socket_list){
            send(i, buf, MESSAGE_LEN, 0);
        }
        memset(buf, 0, MESSAGE_LEN);
    }
}

// server
void Server(std::list<int>& socket_list) {
    LOG(ERROR) << "Server Running...";
    std::map<int, std::string> user_db;      // 用户数据库
    struct timeval timeout;
    while(true) {
        // 在for循环内部，select会监听所有用户的发言，并发送给其他的用户
        for (auto i : socket_list) {
            fd_set rfds;        // select监听的IO多路复用的数据空间
            FD_ZERO(&rfds);
            int maxfd = 0;      // select监听的文件句柄的上限数
            FD_SET(i, &rfds);
            if(maxfd < i){
                maxfd = i;
            }
            // select调用完后，timeout的值可能被修改，是一个不确定的值，所以cpu可能飙升到100%
            // 所以需要在while循环内部每次都赋值一遍
            timeout.tv_sec = 1;
            timeout.tv_usec = 0;
            int rtn = select(maxfd+1, &rfds, NULL, NULL,  /*等待时间*/ &timeout);
            if (rtn < 0) {
                LOG(ERROR) << "select error.";
                cout << "select error." << endl;
            } else if (rtn == 0) {
                // 连接超时
                // LOG(ERROR) << "select overtime, continue";
                // cout << "select OK." << endl;
            } else {
                // recv
                char send_msg[MESSAGE_LEN];
                char recv_msg[MESSAGE_LEN];
                memset(send_msg, 0, MESSAGE_LEN);
                memset(recv_msg, 0, MESSAGE_LEN);
                int len = recv(i, recv_msg, MESSAGE_LEN, 0);
                if (len != 0) {         // 没有客户端连接的时候，不要打印数据
                    LOG(INFO) << "recv message len=" << strlen(recv_msg) << ", clientid=" << i
                         << ", message=" << recv_msg;
                }
                std::string recv_msg_str(recv_msg);
                const std::string name_prefix(SEND_NAME_PREFIX);
                const std::string say_to_client_prefix(SAY_TO);
                const std::string all_user(ALL_USER);
                auto send_socket_list = socket_list;

                if (len < 0) {
                    // 连接出错
                    continue;
                } else if (len == 0) {
                    // 连接关闭
                    cout << "用户" << user_db[i] << "下线了" << endl;
                    user_db.erase(i);
                    close(i);
                    // it正常情况下是一个list，但是本环境下，it只会有一个元素，所以不需要for循环了
                    auto it = std::find(socket_list.begin(), socket_list.end(), i);
                    if (it != socket_list.end())
                        socket_list.erase(it);
                } else {
                    // 下面是各个功能
                    if (boost::starts_with(recv_msg_str, name_prefix)) {
                        // 接受用户注册名字
                        LOG(ERROR) << "origin name=" << recv_msg_str;
                        std::string name = recv_msg_str.substr(name_prefix.size());
                        LOG(ERROR) << "name=" << name;
                        cout << "name=" << name << endl;
                        if (user_db.find(i) == user_db.end()) {
                            user_db.insert(std::pair<int, std::string>(i, name));
                        }
                    } else if (boost::starts_with(recv_msg_str, all_user)) {
                        // TODO 查询所有用户
                        std::string all_user_str;
                        for (auto user : user_db) {
                            all_user_str += user.second;
                            all_user_str += " ";
                        }
                        all_user_str += '\n';
                        cout << all_user << ": " << all_user_str << endl;
                        strcpy(send_msg, all_user_str.c_str());
                        send(i, send_msg, MESSAGE_LEN, 0);
                    } else if (boost::starts_with(recv_msg_str, say_to_client_prefix)) {
                        // TODO 和指定的用户说话
                    } else {
                        // 默认功能，和其他用户聊天
                        for(auto j : send_socket_list){
                            std::string name = user_db[i];
                            if (i != j) {
                                strcpy(send_msg, ("user " + name + ": " + std::string(recv_msg)).c_str());
                                send(j, send_msg, MESSAGE_LEN, 0);
                            }
                        }
                    }  // 所有功能执行完毕

                    cout << "user " << user_db[i] <<" says: " << recv_msg_str << endl;
                }
            }
        }
        // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

int main() {
    std::string workspace_name = "wechat_server";
    wechat::GlogUtil::init_glog(workspace_name);
    LOG(ERROR) << "wechat is running...";

    // socket
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    int reuse = 0;
#ifdef debug_wechat
    reuse = 1;
    cout << __FILE__ << "," << __LINE__ << " debug" << endl;
    LOG(ERROR) << "debug_wechat mode.";
#endif
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr(IP);

    // bind
    int saddr_len = sizeof(serv_addr);
    if (bind(fd, (struct sockaddr*) &serv_addr, saddr_len) == -1) {
        perror("bind error.");
        exit(1);
    }
    LOG(ERROR) << "bind success.";

    // listen
    int listen_queue = 20;  // need less than 30
    if (listen(fd, listen_queue) == -1) {
        perror("listen error.");
        exit(1);
    }
    LOG(ERROR) << "listen success.";

    // accept
    std::list<int> socket_list;
    std::map<int, std::string> user_database;       // 使用fd作为唯一主键，名字是值
    std::string data;       // 线程之间同步数据
    std::thread accept_socket(AcceptSocket, fd, std::ref(serv_addr), saddr_len, std::ref(socket_list));
    accept_socket.detach();
    LOG(ERROR) << "accept socket thread detached.";

    // server
    std::thread server(Server, std::ref(socket_list));
    server.detach();
    LOG(ERROR) << "server thread detached.";

    // TODO: 可以接受命令行指令，执行查看等操作
    while (true) {
        LOG(ERROR) << "server while true...";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000*100));
    }

    return 0;
}