#define CPPCHESSENGINE_DEBUG
#define CPPCHESSENGINE_PRINT

#include <iostream>
#include <string>
#include "board.hpp"
#include "engine/algorithms/eval.hpp"

#include <vector>
#include "engine/algorithms/move_gen.hpp"



int main() {
#ifndef CPPCHESSENGINE_DEBUG
    std::cout << "Enter FEN String: " ;
    std::string input;
    std::getline(std::cin, input);
#else
    std::string input = "rnbqkbnr/pppppppp/8/8/8/8/1PPPPPP1/RNBQKBNR w KQkq - 0 1";
#endif

    try {
        board b1 = board(input);
#ifdef CPPCHESSENGINE_PRINT
        std::cout << __FILE__ << ":" << __LINE__ << " (" << __func__ << ") " << "\n";

        std::cout << "White: " << std::endl;
        auto white = b1.get_white_bitboards();
        for (const auto board : white) {
            b1.print_bitboard(board);
        }

        std::cout << "Black: " << std::endl;
        auto black = b1.get_black_bitboards();
        for (const auto board : black) {
            b1.print_bitboard(board);
        }
        std::cout << "Evaluation: " << eval::get_eval(b1) << "\n";

        std::vector<uint32_t> moves;
        move_gen::generate_legal_moves(b1, moves);
#endif


        } catch (std::invalid_argument &e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}