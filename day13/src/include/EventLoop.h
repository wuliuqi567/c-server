//
// Created by wuliuqi on 2023/2/26.
//
#pragma once
#include "Macros.h"

#include <functional>
#include "Channel.h"
class EventLoop{
public:
    EventLoop();
    ~EventLoop();
    DISALLOW_COPY_AND_MOVE(EventLoop);

    void loop();
    void UpdateChannel(Channel *ch);
private:
    Epoll *ep_{nullptr};
    bool quit {false};
};
