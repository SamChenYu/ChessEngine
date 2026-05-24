#include <ctime>
#include <iostream>
#include <string>

#include <board.hpp>
#include <algorithms/move_gen.hpp>
#include <engine.hpp>

#define AUTOFEN

int main() {
#ifndef AUTOFEN
    std::cout << "Enter FEN String: " ;
    std::string input;
    std::getline(std::cin, input);
#else
    std::string input = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
#endif

    try {

        board b1 = board(input);
        clock_t start = clock();
        auto best_move{engine::start_minimax(b1, 3)};
        clock_t end = clock();
        double time_taken = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000.0;
        std::cout << "Time taken: " << time_taken << " ms" << std::endl;
        move_gen::print_move(best_move);

        } catch (std::invalid_argument &e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}