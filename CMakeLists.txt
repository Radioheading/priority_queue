cmake_minimum_required(VERSION 3.16)
project(priority_queue)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(priority_queue
        data/one.memcheck/code.cpp
        src/exceptions.hpp
        src/priority_queue.hpp
        src/utility.hpp)
