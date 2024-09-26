cmake_minimum_required(VERSION 3.22)
project(FSMng)

set(CMAKE_CXX_STANDARD 20)

add_executable(FSMng main.cpp
        FSM.cpp
        FSM.h)
