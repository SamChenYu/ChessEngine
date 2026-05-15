#include "engine.hpp"

#include <limits>

#include "algorithms/eval.hpp"
#include "algorithms/move_gen.hpp"

int engine::max_depth = 5;

float engine::minimax(const board& b, int depth, float alpha, float beta, bool is_maximising) {

    if (depth == engine::max_depth)
        return eval::get_eval(b);

    std::vector<uint32_t> moves;
    move_gen::generate_legal_moves(b, moves);

    if (is_maximising) {
        float best_val = std::numeric_limits<float>::lowest();


        for (auto move: moves) {
            board new_board{b}; // New allocation
            new_board.make_move(move);
            float temp_val = minimax(new_board, depth + 1, alpha, beta, false);

            if (temp_val >= best_val) {
                best_val = temp_val;
            }
            if (alpha >= best_val) {
                alpha = best_val;
            }

            // Pruning
            if (beta <= alpha)
                break;
        }
        return best_val;
    } else {
        float least_val = std::numeric_limits<float>::max();
        for (auto move: moves) {
            board new_board{b}; // New allocation
            new_board.make_move(move);
            float temp_val = minimax(new_board, depth + 1, alpha, beta, true);
            // revert move here if I am add it later

            if (temp_val <= least_val) {
                least_val = temp_val;
            }
            // Confirm if it isn't actually just:
            // beta = std::max(alpha, least_val);
            beta = std::min(beta, least_val);

            if (beta <= alpha)
                break;
        }
        return least_val;
    }
}