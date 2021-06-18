cmake_minimum_required(VERSION 3.19)
project(test)

set(CMAKE_CXX_STANDARD 17)

add_executable(n_puzzle src/cpp/main.cpp src/cpp/Matrix.cpp src/cpp/Puzzle.cpp src/cpp/aStar.cpp)
target_compile_options(n_puzzle PRIVATE -Werror -Wall -Wextra -Wno-sign-compare -O2)
target_link_directories(n_puzzle PRIVATE src/cpp/)