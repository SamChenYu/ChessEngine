#define CPPCHESSENGINE_DEBUG
#include "board.hpp"
#include <iostream>
#include <string>



int main() {
    //std::cout << "Enter FEN String: " ;
    // std::string input;
    // std::getline(std::cin, input);

    std::string input = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    try {
        board b1 = board(input);
        auto white = b1.get_white_bitboards();
        // for (const auto board : white) {
        //     b1.print_bitboard(board);
        // }
        auto black = b1.get_black_bitboards();
        for (const auto board : black) {
            b1.print_bitboard(board);
        }
    } catch (std::invalid_argument &e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}