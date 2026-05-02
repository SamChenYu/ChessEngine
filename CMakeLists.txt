cmake_minimum_required(VERSION 4.0)
project(CppChessEngine)

set(CMAKE_CXX_STANDARD 23)

add_executable(CppChessEngine
       src/main.cpp
        external/magic-bits/include/magic_bits.hpp
        src/move_gen/move_gen.h
        src/board.h
        src/board.cpp
)
