cmake_minimum_required(VERSION 3.22.1)
project(BigramsTrigrams_Parallel)

set(CMAKE_CXX_STANDARD 17)

add_executable(BigramsTrigrams_Parallel main.cpp
        WordNgrams.cpp
        WordNgrams.h
)
