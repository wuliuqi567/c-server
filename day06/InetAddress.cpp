#include "InetAddress.h"
#include <string.h>

InetAddress::InetAddress():addr_len(sizeof (add)){
    bzero(&add, sizeof (add));
};

InetAddress::InetAddress(char *ip, uint16_t port) {
    bzero(&add, sizeof (add));
    addr.sin_family = AF_INET;
    addr.sin_port = port;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr_len = sizeof (add);
}

InetAddress::~InetAddress() {

}