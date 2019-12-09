#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>

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

std::mutex save_mutex;

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
            // fd_set是select进行IO多路复用的数据空间
            fd_set rfds;    
            FD_ZERO(&rfds);
            int maxfd = 0;
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
                cout << "recv message: " << buf << ", len=" << len << endl;
                // {
                //     std::unique_lock<std::mutex> l(save_mutex);
                //     data(buf);
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
        //     std::unique_lock<std::mutex> l(save_mutex);
        //     buf = data.c_str();
        // }
        // if (sizeof(buf) == 0) {
        //     cin >> buf;
        // }
        cin >> buf;
        for(auto i : socket_list){
            send(i, buf, MESSAGE_LEN, 0);
        }
        memset(buf, 0, MESSAGE_LEN);
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
    std::string data;
    std::thread accept_socket(AcceptSocket, fd, std::ref(serv_addr), saddr_len, std::ref(socket_list));
    accept_socket.detach();
    cout << "accept done" << endl;

    // recv
    std::thread recv_data(RecvData, std::ref(socket_list), std::ref(data));
    recv_data.detach();
    cout << "recv done" << endl;

    // send
    std::thread send_data(SendData, std::ref(socket_list), std::ref(data));
    send_data.detach();
    cout << "send done" << endl;

    while(true) {
        // sleep(1);
    }

    return 0;
}