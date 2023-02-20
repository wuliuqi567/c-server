#include "Socket.h"
#include "InetAddress.h"
#include "util.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>


Socket::Socket(){
    fd = socket(AF_INET, SOCK_STREAM, 0);
    errif(fd == -1, "socket create error");
}
Socket::~Socket() {
    if (fd != -1){
        close(fd);
        fd = -1;
    }

}
Socket::Socket(int clnt_socket) {
    errif(clnt_socket == -1, "clnt socket create error");
}

void Socket::bind(InetAddress *addr) {
    errif(::bind(fd, (sockaddr*)addr->addr, addr->addr_len) == -1, "bind error");
}

int Socket::getFd() {
    return fd;
}

void Socket::listen() {
    errif(::listen(fd, SOMAXCONN) == -1, "listen socket error");
}

int Socket::accept(InetAddress * addr) {
    int clnt_socket = ::accept(fd, (sockaddr*) addr->addr, &addr->addr_len);
    errif(clnt_socket == -1, "socket accept error");
    return clnt_socket;
}