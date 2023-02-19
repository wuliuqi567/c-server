#pragma one

class InetAddress;
class Socket{
private:
    int fd;

public:
    Socket(){};
    Socket(int);
    ~Socket(){};

    void bind(InetAddress*);
    void listen();
    void accept(InetAddress*);
    void getFd();
};