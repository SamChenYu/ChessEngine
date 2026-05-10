#define CPPCHESSENGINE_DEBUG
// #define CPPCHESSENGINE_PRINT

#include <iostream>
#include <string>
#include "board.hpp"
#include "engine/algorithms/eval.hpp"

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
    std::cout << "Evaluation: " << eval::get_eval(b1) << "\n";
    } catch (std::invalid_argument &e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}