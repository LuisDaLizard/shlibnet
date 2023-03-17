cmake_minimum_required(VERSION 3.1)
project(shlibnet)

set(CMAKE_CXX_STANDARD 20)

set(SHLIBNET_SRC
        "include/shlibnet/Socket.h"
        "src/shlibnet/Socket.cpp"
        )

add_library(shlibnet ${SHLIBNET_SRC})

target_include_directories(shlibnet
        PRIVATE "include")
