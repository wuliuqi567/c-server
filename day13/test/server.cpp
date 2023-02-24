//
// Created by wuliuqi on 2023/2/24.
//
#include "Server.h"
#include "EventLoop.h"

int main(){

    EventLopp *loop = new Event_loop();
    Server *server = new Server();
    loop->Loop();
    delete server;
    delete loop;
    return 0;
}