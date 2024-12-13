cmake_minimum_required(VERSION 3.29)
project(WuZiQi)

set(CMAKE_CXX_STANDARD 20)

add_executable(WuZiQi main.cpp
        Player.cpp
        Player.h
        Game.cpp
        Game.h
        Board.cpp
        Board.h)

include_directories(./cpp)
