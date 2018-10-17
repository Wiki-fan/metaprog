cmake_minimum_required(VERSION 3.12)
project(patterns)

set(CMAKE_CXX_STANDARD 17)

add_executable(patterns A.h A.cpp B.h B.cpp BaseC.h C.h C.cpp main.cpp)
