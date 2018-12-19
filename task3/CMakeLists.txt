cmake_minimum_required(VERSION 3.13)
project(task3)

set(CMAKE_CXX_STANDARD 17)

add_executable(task3 typelists-m.h TestData.h Reader1.h Reader2.h Reader3.h main.cpp)
