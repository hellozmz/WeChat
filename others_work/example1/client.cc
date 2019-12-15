#include<iostream>
using namespace std;
#include <unistd.h>
#include <arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<signal.h>
#include<cstring>
#include<cstdlib>
#define BUFMAX 1024
 
struct packet
{
    int len;
    char name[16];
    char buf[BUFMAX];
};
 
void test(pid_t pid,int num)
{
    if(num<=0)
     {
          if(num==0)
            {
                 cout<<"连接中断  read进程关闭"<<endl;
                 kill(pid,SIGUSR1);
                 exit(0);
             }
            else 
             {
                 cout<<"num<0错误"<<endl;
                 exit(0);
              }
      }
 
}
 
 
 
void func(int flag)
{
    if(flag==SIGUSR1)
    {
        cout<<"连接中断 write进程中断"<<endl;
        exit(0);
    }
}
 
 
 
 ssize_t readn(int fd, void *buf, size_t count)
    {
        ssize_t nleft=count;
        ssize_t nread;
        char *bufp=(char *)buf;
    
        while(nleft>0)
        {
            if( (nread=read(fd,bufp,nleft))<=0 )
            {
                cout<<"*****nead"<<nread<<endl;
                return 0;
            }
            nleft-=nread;
            bufp+=nread;
 
        }
        return count;
    }
 
 
 
ssize_t writen(int fd, void *buf, size_t count)
        {
                ssize_t nleft=count;
                ssize_t nwrite;
                char *bufp=(char *)buf;
        cout<<"**************"<<endl;
                while(nleft>0)
                {
                        if( (nwrite=write(fd,bufp,nleft))<=0 )
                        {
                              cout<<"连接中断"<<endl;
                              return -1;
                        }
                        nleft-=nwrite;
                        bufp+=nwrite;
 
                }
                return count;
        }
 
 
 
int main()
{
    cout<<"程序运行"<<endl;
    struct packet pack;
    memset(&pack ,0,sizeof(pack));
    int num,nlen;
    pid_t pid;
    int connectfd;
    struct sockaddr_in servaddr;
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    //servaddr.sin_addr.s_addr=inet_addr("47.107.36.224");      //服务器 ip地址
    //servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(3389);
    if( (connectfd=socket(AF_INET,SOCK_STREAM,0))==-1 )
        cout<<"socker错误"<<endl;
 
 
    if( connect(connectfd,(struct sockaddr*)&servaddr,sizeof(servaddr))==-1 )   
        cout<<"连接失败"<<endl;
    else
    cout<<"连接成功"<<endl;
    
    //strcpy(pack.name,"cli123 ");
    cout<<endl<<endl<<"请取一个名字  名字不要重复，不要有空格"<<endl<<"       ";
    cin>>pack.name;
 
 
    pid=fork();
    if(pid>0)
    {
        while(1)
        {
            num=readn(connectfd,&pack,4);
            test(pid,num);
            nlen=ntohl(pack.len);
            num=readn(connectfd,pack.name,nlen+16);
            test(pid,num);
 
            cout<<pack.name<<" : "<<pack.buf<<endl;;
            memset(&pack,0,strlen(pack.buf)+20);
        }
    }
    else if(pid==0)
    {
        write(connectfd,pack.name,strlen(pack.name));
        signal(SIGUSR1,func);
        while(1)
        {
            cin>>pack.buf; 
            nlen=strlen(pack.buf)+1;    //  '\0'
            pack.len=htonl(nlen);
            write(connectfd,&pack,nlen+20);
            //memset(pack.buf,0,nlen);
        }
    }
    else
    cout<<"pid错误"<<endl;
 
 
    return 0;
}
