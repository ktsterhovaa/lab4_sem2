cmake_minimum_required(VERSION 3.27)
project(lab4)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab4
        Common.hpp
        main.cpp
        menu.h
        BinaryTree.h

)
add_subdirectory(Google_tests)