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
    int accept(InetAddress*);
    void getFd();
};