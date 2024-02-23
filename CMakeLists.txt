cmake_minimum_required(VERSION 3.22.1)
project(BigramsTrigrams_Parallel)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Xpreprocessor -fopenmp -lomp")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lomp")
# the following 2 lines are needed on macOS. Check the position of headers in libraries (e.g. Macports vs. Homebrew)
include_directories(/usr/local/opt/libomp/include)
link_directories(/usr/local/opt/libomp/lib)
add_executable(BigramsTrigrams_Parallel main.cpp
        WordNgrams.cpp
        WordNgrams.h
        CharacterNgrams.cpp
        CharacterNgrams.h
)
