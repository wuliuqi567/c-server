//
// Created by wuliuqi on 2023/2/24.
//

#pragma one
#include "Macros.h"

#include <vector>

#ifdef OS_LINUX
#include <sys/epoll.h>
#endif

class Epoll{

public:
    Epoll();
    ~Epoll();
    DISALLOW_COPY_AND_MOVE(Epoll);

    void UpdateChannel(Channel* ch);
    void DeleteChannel(Channel* ch);
    std::vector<Channel *> Poll(int timeout = -1);

private:
    int epfd_{-1};
    struct epoll_event *events_{nullptr};

};


#endif //WEBSERVER_EPOLL_H
