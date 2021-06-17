cmake_minimum_required(VERSION 3.19)
project(test)

set(CMAKE_CXX_STANDARD 14)

add_executable(n_puzzle src/cpp/main.cpp src/cpp/Puzzle.tpp src/cpp/Puzzle.hpp src/cpp/aStar.tpp)
#target_compile_options(n_puzzle PRIVATE -Werror -Wall -Wextra)
