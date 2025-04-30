cmake_minimum_required(VERSION 3.26)
project(assignment3)

set(CMAKE_CXX_STANDARD 17)

add_executable(assignment3 main.cpp
        TextAnalyser.h
        TextAnalyser.cpp
        Node.h
        Node.cpp
        UnsortedWordList.h
        UnsortedWordList.cpp)
