//
// Created by wuliuqi on 2023/2/26.
//

#include "include/Channel.h"

#include <sys/epoll.h>
#include <unistd.h>

#include <utility>

#include "EventLoop.h"
#include "Socket.h"

Channel::Channel(EventLoop *loop, int fd)
    :loop_(loop), fd_(fd), listen_events_(0), ready_events_(0), in_epoll_(false){};

Channel::~Channel() {
    if (fd_!=-1){
        close(fd_);
        fd_ = -1;
    }
}

void Channel::HandleEvent() {
    if (ready_events_ & (EPOLLIN | EPOLLPRI)){
        read_callback_();
    }
    if (ready_events_ &(EPOLLOUT)){
        write_callback_();
    }
}

void Channel::EnableRead() {
    listen_events_ |= EPOLLIN | EPOLLPRI;
    loop_->UpdateChannel(this);
}

void Channel::UseET() {
    listen_events_ |= EPOLLET;
    loop_->UpdateChannel(this);

}

int Channel::GetFd() {return fd_;}

uint32_t Channel::GetReadyEvents() {return ready_events_;}

uint32_t Channel::GetListenEvents() {return listen_events_;}

bool Channel::GetInEpoll() {return in_epoll_;}

void Channel::SetInEpoll(bool in) {in_epoll_ = in;}

void Channel::SetReadCallback(const std::function<void()> &callback) {
    read_callback_ = callback;
}