// #define CPPCHESSENGINE_DEBUG
//#defirnbqkbnr/1ppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1ne CPPCHESSENGINE_PRINT

#include "board.hpp"
#include "engine/eval/eval.hpp"
#include <iostream>
#include <string>

int main() {

#ifndef CPPCHESSENGINE_DEBUG
    std::cout << "Enter FEN String: " ;
     std::string input;
     std::getline(std::cin, input);
#else
    std::string input = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
#endif

    try {
        board b1 = board(input);
        eval e;
        std::cout << "Evaluation: " << e.get_eval(b1) << "\n";

#ifdef CPPCHESSENGINE_PRINT
        auto white = b1.get_white_bitboards();
        for (const auto board : white) {
            b1.print_bitboard(board);
        }
        auto black = b1.get_black_bitboards();
        for (const auto board : black) {
            b1.print_bitboard(board);
        }
#endif
    } catch (std::invalid_argument &e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}