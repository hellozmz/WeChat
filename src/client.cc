#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>

#include <iostream>

#define IP "127.0.0.1"
#define PORT 9977
#define MESSAGE_LEN 1024

using std::cin;
using std::cout;
using std::endl;

int main() {
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
                cout << "recv data len=" << strlen(buf) << ", clientid=" << fd << ", message=" << buf << endl;
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

    return 0;
}