#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "util.h"
#include <stdlib.h>
#include<sys/epoll.h>
#include <errno.h>


// #include "Epoll.h"
// #include "InetAddress.h"
// #include "Socket.h"

#define MAX_EVENTS 1024
#define READ_BUFFER 1024
void setnonblocking(int fd){
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}
void errif(bool condition, const char *essmg);

int main() {
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(socketfd == -1, "socket create error");

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htonl(8888);

    errif(bind(socketfd, (sockaddr*)&server_addr, sizeof(server_addr))==-1, "socket bind error");
    errif(listen(socketfd, SOMAXCONN) == -1, "socket listen error");
    
    sockaddr_in clnt_addr;
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    // int client_socketfd = accept(socketfd, (sockaddr*)&clnt_addr, &clnt_addr_len);

    int epfd = epoll_create(0);
    errif(epfd == -1, "epoll create error");
    
    struct epoll_event events[MAX_EVENTS], ev;
    bzero(&events, sizeof(events));
    bzero(&ev, sizeof(ev));

    ev.events = EPOLLIN|EPOLLET;
    ev.data.fd = socketfd;
    setnonblocking(socketfd);
    epoll_ctl(epfd, EPOLL_CTL_ADD, socketfd, &ev);

    while(true){
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
        errif(nfds == -1, "epoll wait error");

        for(int i = 0; i < nfds; i++){
            if(events[i].data.fd == socketfd){ // new connect comes in 
                struct sockaddr_in clnt_addr;
                bzero(&clnt_addr, sizeof(clnt_addr));
                socklen_t clnt_addr_len;
                bzero(&clnt_addr_len, sizeof(clnt_addr_len));

                int clnt_sockfd = accept(socketfd, (sockaddr*)clnt_addr, &clnt_addr_len);

                bzero(&ev, sizeof(ev));
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = clnt_sockfd;

                setnonblocking(clnt_sockfd);
                epoll_ctl(epfd, EPOLLIN, clnt_sockfd, &ev);
            }
            else if(events[i].events & EPOLLIN){      //可读事件
                char buf[READ_BUFFER];
                while(true){    //由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
                    bzero(&buf, sizeof(buf));
                    ssize_t bytes_read = read(events[i].data.fd, buf, sizeof(buf));
                    if(bytes_read > 0){
                        printf("message from client fd %d: %s\n", events[i].data.fd, buf);
                        write(events[i].data.fd, buf, sizeof(buf));
                    } else if(bytes_read == -1 && errno == EINTR){  //客户端正常中断、继续读取
                        printf("continue reading");
                        continue;
                    } else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//非阻塞IO，这个条件表示数据全部读取完毕
                        printf("finish reading once, errno: %d\n", errno);
                        break;
                    } else if(bytes_read == 0){  //EOF，客户端断开连接
                        printf("EOF, client fd %d disconnected\n", events[i].data.fd);
                        close(events[i].data.fd);   //关闭socket会自动将文件描述符从epoll树上移除
                        break;
                    }
                }
            } else{         //其他事件，之后的版本实现
                printf("something else happened\n");
            }
        }
    }
    close(socketfd);
    return 0;
}

void errif(bool condition, const char *essmg){
    if (condition){
        perror(essmg);
        exit(EXIT_FAILURE);
    }
}
