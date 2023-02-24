//
// Created by wuliuqi on 2023/2/24.
//

#include "include/Socket.h"
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include "include/util.h"
InetAddress::InetAddress()=default;
InetAddress::InetAddress(const char *ip, uint16_t port) {
    memset(&addr_, 0, sizeof (addr_));
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = inet_addr(ip);
    addr_.sin_port = htons(port);
}

sockaddr_in InetAddress::GetAddr() {return addr_;}

const char *InetAddress::GetIP() {return inet_ntop(addr_.sin_addr.s_addr);}

uint16_t InetAddress::GetPort() {return ntohs(addr_.sin_port);}

void InetAddress::SetAddr(sockaddr_in addr) {addr_ = addr;};

Socket::Socket() {
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    ErrorIf(fd_==-1, "socket create error");
}
Socket::~Socket() {
    if (fd_ != -1) {
        close(fd_);
        fd_ = -1;
    }
}

Socket::Socket(int fd) {
    fd_ = fd;
    ErrorIf(fd_== -1, "socket create error");
}

void Socket::Bind(InetAddress *addr) {
    struct sockaddr_in tmp_addr = addr->GetAddr();
    ErrorIf(bind(fd_, (sockaddr*) &tmp_addr, sizeof (tmp_addr)) == -1, "bind error");
}

void Socket::Listen() {
    ErrorIf(listen(fd_, SOMAXCONN)==-1, "listen error");
}
void Socket::SetNonBlocking() { fcntl(fd_, F_SETFL, fcntl(fd_, F_GETFL) | O_NONBLOCK); }

int Socket::Accept(InetAddress *addr) {
    // for server socket
    int clnt_sockfd = -1;
    struct sockaddr_in tmp_addr {};
    socklen_t addr_len = sizeof(tmp_addr);
    if (fcntl(fd_, F_GETFL) & O_NONBLOCK) {
        while (true) {
            clnt_sockfd = accept(fd_, (sockaddr *)&tmp_addr, &addr_len);
            if (clnt_sockfd == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
                // printf("no connection yet\n");
                continue;
            }
            if (clnt_sockfd == -1) {
                ErrorIf(true, "socket accept error");
            } else {
                break;
            }
        }
    } else {
        clnt_sockfd = accept(fd_, (sockaddr *)&tmp_addr, &addr_len);
        ErrorIf(clnt_sockfd == -1, "socket accept error");
    }
    addr->SetAddr(tmp_addr);
    return clnt_sockfd;
}
void Socket::Connect(InetAddress *addr) {
    // for client socket
    struct sockaddr_in tmp_addr = addr->GetAddr();
    if (fcntl(fd_, F_GETFL) & O_NONBLOCK) {
        while (true) {
            int ret = connect(fd_, (sockaddr *)&tmp_addr, sizeof(tmp_addr));
            if (ret == 0) {
                break;
            }
            if (ret == -1 && (errno == EINPROGRESS)) {
                continue;
                /* 连接非阻塞式sockfd建议的做法：
                    The socket is nonblocking and the connection cannot be
                  completed immediately.  (UNIX domain sockets failed with
                  EAGAIN instead.)  It is possible to select(2) or poll(2)
                  for completion by selecting the socket for writing.  After
                  select(2) indicates writability, use getsockopt(2) to read
                  the SO_ERROR option at level SOL_SOCKET to determine
                  whether connect() completed successfully (SO_ERROR is
                  zero) or unsuccessfully (SO_ERROR is one of the usual
                  error codes listed here, explaining the reason for the
                  failure).
                  这里为了简单、不断连接直到连接完成，相当于阻塞式
                */
            }
            if (ret == -1) {
                ErrorIf(true, "socket connect error");
            }
        }
    } else {
        ErrorIf(connect(fd_, (sockaddr *)&tmp_addr, sizeof(tmp_addr)) == -1, "socket connect error");
    }
}

int Socket::GetFd() { return fd_; }
