cmake_minimum_required(VERSION 3.19)
project(test)

set(CMAKE_CXX_STANDARD 14)

add_executable(n_puzzle src/main.cpp src/Puzzle.tpp src/Puzzle.hpp src/aStar.tpp)
#target_compile_options(n_puzzle PRIVATE -Werror -Wall -Wextra)
