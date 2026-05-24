cmake_minimum_required(VERSION 4.0)
project(CppChessEngine)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Each library is compiled with different debug symbols


# Main executable
add_library(enginelib_cli
        engine/src/board.cpp
        engine/src/engine.cpp
        engine/src/eval.cpp
        engine/src/move_gen.cpp
)

target_include_directories(enginelib_cli PUBLIC
        external/magic-bits/include
        engine/include
)
target_compile_definitions(enginelib_cli PRIVATE ENGINE_DEBUG)

add_executable(cli_chessengine
        apps/cli_main.cpp
)
target_link_libraries(cli_chessengine PRIVATE enginelib_cli)




# Tests executable
add_library(enginelib_tests
        engine/src/board.cpp
        engine/src/engine.cpp
        engine/src/eval.cpp
        engine/src/move_gen.cpp
)
target_include_directories(enginelib_tests PUBLIC
        external/magic-bits/include
        engine/include
)
target_compile_definitions(enginelib_tests PRIVATE
        ENGINE_DEBUG
        BOARD_DEBUG
        MOVEGEN_DEBUG
)

add_executable(chesstests
        tests/test_main.cpp
)
target_link_libraries(chesstests PRIVATE enginelib_tests)
