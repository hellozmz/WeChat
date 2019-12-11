#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>

#include <cstring>
#include <iostream>
#include <list>
#include <mutex>
#include <thread>

#define IP "127.0.0.1"
#define PORT 9977
#define MESSAGE_LEN 1024

using std::cin;
using std::cout;
using std::endl;

std::mutex data_mutex;

// 监听注册的client socket
void AcceptSocket(int skt, struct sockaddr_in s_addr, socklen_t saddr_len,
                  std::list<int>& socket_list) {
    while(true) {
        int fd = accept(skt, (sockaddr*)&s_addr, &saddr_len);
        cout << fd << " is linked." << endl;
        socket_list.push_back(fd);
        // sleep(1);
    }
}

// IO多路复用
void RecvData(std::list<int>& socket_list, std::string& data) {
    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    while(true) {
        for (auto i : socket_list) {
            fd_set rfds;        // select监听的IO多路复用的数据空间
            FD_ZERO(&rfds);
            int maxfd = 0;      // select监听的文件句柄的上限数
            FD_SET(i, &rfds);
            if(maxfd < i){
                maxfd = i;
            }
            int rtn = select(maxfd+1, &rfds, NULL, NULL, &tv);
            if (rtn == -1) {
                cout << "select error." << endl;
            } else if (rtn == 0) {
                // OK
                // cout << "select OK." << endl;
            } else {
                // recv
                char buf[MESSAGE_LEN];
                memset(buf, 0, MESSAGE_LEN);
                int len = recv(i, buf, MESSAGE_LEN, 0);
                cout << "recv message len=" << strlen(buf) << ", clientid=" << i << ", message=" << buf << endl;
                // {
                //     std::lock_guard<std::mutex> lck(data_mutex);
                //     data.assign(buf);
                //     cout << "RecvData data=" << data << endl;
                // }
            }
        }
        sleep(1);
    }
}

// send 所有的用户都发送数据
void SendData(std::list<int>& socket_list, std::string& data) {
    while(true) {
        char buf[MESSAGE_LEN];
        // {
        //     std::lock_guard<std::mutex> lck(data_mutex);
        //     cout << "SendData data=" << data << endl;
        //     for (int i=0; i<data.size(); ++i) {
        //         buf[i] = data[i];
        //     }
        //     buf[data.size()] = '\0';
        // }
        // if (data.size() == 0) {
        //     cin >> buf;
        // }
        // data = "";
        cin >> buf;
        for(auto i : socket_list){
            send(i, buf, MESSAGE_LEN, 0);
        }
        memset(buf, 0, MESSAGE_LEN);
    }
}

// server
void Server(std::list<int>& socket_list) {
    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    while(true) {
        for (auto i : socket_list) {
            fd_set rfds;        // select监听的IO多路复用的数据空间
            FD_ZERO(&rfds);
            int maxfd = 0;      // select监听的文件句柄的上限数
            FD_SET(i, &rfds);
            if(maxfd < i){
                maxfd = i;
            }
            int rtn = select(maxfd+1, &rfds, NULL, NULL, &tv);
            if (rtn == -1) {
                cout << "select error." << endl;
            } else if (rtn == 0) {
                // OK
                // cout << "select OK." << endl;
            } else {
                // recv
                char send_msg[MESSAGE_LEN];
                char recv_msg[MESSAGE_LEN];
                memset(send_msg, 0, MESSAGE_LEN);
                memset(recv_msg, 0, MESSAGE_LEN);
                int len = recv(i, recv_msg, MESSAGE_LEN, 0);
                cout << "recv message len=" << strlen(recv_msg) << ", clientid=" << i << ", message=" << recv_msg << endl;

                for(auto j : socket_list){
                    if (i != j) {
                        strcpy(send_msg, ("user " + std::to_string(i) + ": " + std::string(recv_msg)).c_str());
                        send(j, send_msg, MESSAGE_LEN, 0);
                    }
                }
            }
        }
        sleep(1);
    }
}

int main() {
    cout << "server is running..." << endl;

    // socket
    int fd = socket(AF_INET, SOCK_STREAM, 0);
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

    // listen
    int listen_queue = 20;  // need less than 30
    if (listen(fd, listen_queue) == -1) {
        perror("listen error.");
        exit(1);
    }

    // accept
    std::list<int> socket_list;
    std::string data;       // 线程之间同步数据
    std::thread accept_socket(AcceptSocket, fd, std::ref(serv_addr), saddr_len, std::ref(socket_list));
    accept_socket.detach();
    cout << "accept done" << endl;

    // // recv
    // std::thread recv_data(RecvData, std::ref(socket_list), std::ref(data));
    // recv_data.detach();
    // cout << "recv done" << endl;

    // // send
    // std::thread send_data(SendData, std::ref(socket_list), std::ref(data));
    // send_data.detach();
    // cout << "send done" << endl;

    // server
    std::thread server(Server, std::ref(socket_list));
    server.detach();

    while(true) {
        // sleep(1);
    }

    return 0;
}