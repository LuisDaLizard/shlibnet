cmake_minimum_required(VERSION 3.23)
project(ShlibNet)

set(CMAKE_CXX_STANDARD 20)

set(SHLIBNET_SRC
        "include/ShlibNet/Socket.h"
        "src/ShlibNet/Socket.cpp"
        )

add_library(ShlibNet ${SHLIBNET_SRC})

target_include_directories(ShlibNet
        PRIVATE "include")