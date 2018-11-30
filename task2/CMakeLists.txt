cmake_minimum_required(VERSION 3.12)
project(task2)

set(CMAKE_CXX_STANDARD 17)

add_executable(task2 typelists-m.h hierarchy-m.h abstract_factory_gen-m.h
        functor_dispatch.h TypeInfo.h Functor.h ExampleTypeLists.h
        main.cpp)
