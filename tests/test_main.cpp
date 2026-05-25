#include "test_board.hpp"
#include "test_engine.hpp"
#include "test_movegen.hpp"



#include <iostream>

int main() {

    std::cout << "Board tests" << std::endl;
    test_board::execute_tests();

    std::cout << "MoveGen tests" << std::endl;
    test_movegen::execute_tests();

    std::cout << "Engine tests" << std::endl;
    test_engine::execute_tests();
    return 0;
}