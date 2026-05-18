#define CPPCHESSENGINE_DEBUG
#define CPPCHESSENGINE_PRINT
// #define CPPCHESSENGINE_MOVE_DEBUG

#include <iostream>
#include <string>
#include <ctime>
#include "board.hpp"
#include "engine/algorithms/eval.hpp"

#include <vector>

#include "engine/algorithms/move_gen.hpp"
#include "engine/engine.hpp"


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
        std::cout << __FILE__ << ":" << __LINE__ << " (" << __func__ << ") " << "\n";

        // std::cout << "White: " << std::endl;
        // auto white = b1.get_white_bitboards();
        // for (const auto board : white) {
        //     b1.print_bitboard(board);
        // }
        //
        // std::cout << "Black: " << std::endl;
        // auto black = b1.get_black_bitboards();
        // for (const auto board : black) {
        //     b1.print_bitboard(board);
        // }



        // std::cout << "White occupancy: " << std::endl;
        // b1.print_bitboard(b1.get_white_occupancy_board());
        // std::cout << "Black occupancy: s " << std::endl;
        // b1.print_bitboard(b1.get_black_occupancy_board());
        //
        //
        // std::cout << "Evaluation: " << eval::get_eval(b1) << "\n";

        // std::cout << "Moves: " << std::endl;
        // std::vector<uint32_t> moves;
        // move_gen::generate_legal_moves(b1, moves);
        //
        // std::cout << "Enemy attack bitboards" << std::endl;
        // auto attacks = move_gen::generate_enemy_attack_bitboard(b1);
        // b1.print_bitboard(attacks);
        //
        // std::cout << "Mailbox " << std::endl;
        // b1.print_mailbox();


        clock_t start = clock();
        auto best_move{engine::start_minimax(b1, 7)};
        clock_t end = clock();
        double time_taken = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000.0;
        std::cout << "Time taken: " << time_taken << " ms" << std::endl;
        move_gen::print_move(best_move);



#endif


        } catch (std::invalid_argument &e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}