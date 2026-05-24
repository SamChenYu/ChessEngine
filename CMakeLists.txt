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
        engine/include/config.hpp
)

target_include_directories(enginelib PUBLIC
        external/magic-bits/include
        engine/include
)



# Main application
add_executable(cli_chessengine
        apps/cli_main.cpp
)
target_link_libraries(cli_chessengine PRIVATE enginelib)
target_compile_definitions(cli_chessengine PRIVATE
        ENGINE_DEBUG
)



# Tests executable
add_executable(chesstests
        tests/test_main.cpp
)
target_link_libraries(chesstests PRIVATE enginelib)
target_compile_definitions(chesstests PRIVATE
        ENGINE_DEBUG, BOARD_DEBUG, MOVEGEN_DEBUG
)