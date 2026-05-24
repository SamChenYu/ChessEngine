cmake_minimum_required(VERSION 4.0)
project(CppChessEngine)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Shared engine/library code
add_library(enginelib
        engine/src/board.cpp
        engine/src/engine.cpp
        engine/src/eval.cpp
        engine/src/move_gen.cpp
)

target_include_directories(enginelib PUBLIC
        external/magic-bits/include
        engine/include
        apps
)

# Main application
add_executable(CppChessEngine
        apps/main.cpp
)

target_link_libraries(CppChessEngine PRIVATE enginelib)

# Tests executable
add_executable(ChessTests
        tests/test_main.cpp
)

target_link_libraries(ChessTests PRIVATE enginelib)