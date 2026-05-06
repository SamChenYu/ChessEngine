cmake_minimum_required(VERSION 4.0)
project(CppChessEngine)

set(CMAKE_CXX_STANDARD 23)

add_executable(CppChessEngine

        external/magic-bits/include/magic_bits.hpp
        src/board.hpp
        src/board.cpp
        src/main.cpp
        src/engine/eval/eval.hpp
        src/engine/eval/eval.cpp
        src/engine/engine.hpp
        src/engine/engine.cpp
        src/engine/eval/pst.hpp
)
