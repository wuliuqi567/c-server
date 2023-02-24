cmake_minimum_required(VERSION 3.23)
project(day02)

set(CMAKE_CXX_STANDARD 14)

include_directories(c-server/day02)

add_executable(webserver
        c-server/day02/client.cpp
        c-server/day02/Epoll.cpp
        c-server/day02/Epoll.h
        c-server/day02/InetAddress.cpp
        c-server/day02/InetAddress.h
        c-server/day02/server.cpp
        c-server/day02/Socket.cpp
        c-server/day02/Socket.h
        c-server/day02/util.cpp
        c-server/day02/util.h)
