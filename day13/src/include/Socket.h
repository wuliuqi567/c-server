//
// Created by wuliuqi on 2023/2/24.
//

#ifndef WEBSERVER_SOCKET_H
#define WEBSERVER_SOCKET_H

#include <arpa/inet.h>
#include "Macros.h"

class InetAddress{
public:
    InetAddress();
    InetAddress(const char *ip, uint16_t port);
    ~InetAddress();

    void SetAddr(sockaddr_in addr);

    sockaddr_in GetAddr();
    const char *GetIP();
    uint16_t GetPort();

private:
    struct sockaddr_in addr_ {};
};

class Socket{
public:
    Socket();
    Socket(int sock);
    ~Socket();

    void Bind(InetAddress* addr);
    void Listen();
    int Accept(InetAddress* addr);
    int SetNonBlocking();
    int GetFd();
private:
    int fd_{-1};

};
#endif //WEBSERVER_SOCKET_H
