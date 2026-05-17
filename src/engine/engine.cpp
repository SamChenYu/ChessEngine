#define CPPCHESSENGINE_DEBUG

#include "engine.hpp"
#include <limits>
#include "algorithms/eval.hpp"
#include "algorithms/move_gen.hpp"

#ifdef CPPCHESSENGINE_DEBUG
#include <iostream>
#endif

[[nodiscard]]
uint32_t engine::start_minimax(const board& b, int max_depth) {
#ifdef CPPCHESSENGINE_DEBUG
    s_nodes_searched = 0;
#endif
    s_max_depth = max_depth;
    s_best_move_eval = b.m_white_turn ?
        minimax(b, 0, std::numeric_limits<float>::max(), std::numeric_limits<float>::min(), true) :
        minimax(b,0, std::numeric_limits<float>::max(), std::numeric_limits<float>::min(), false);
#ifdef CPPCHESSENGINE_DEBUG
    std::cout << "Nodes searched : " << s_nodes_searched << std::endl;
#endif
    return s_best_move;
}

[[nodiscard]]
float engine::minimax(const board& b, int depth, float alpha, float beta, bool is_maximising) {

    if (depth == engine::s_max_depth)
        return eval::get_eval(b);

#ifdef CPPCHESSENGINE_DEBUG
    s_nodes_searched++;
#endif

    std::vector<uint32_t> moves;
    move_gen::generate_legal_moves(b, moves);

    if (is_maximising) {
        float best_val = std::numeric_limits<float>::lowest();

        for (auto move : moves) {
            board new_board{b};
            new_board.make_move(move);

            float temp_val = minimax(new_board, depth + 1, alpha, beta, false);

            if (temp_val > best_val) {
                best_val = temp_val;

                if (depth == 0)
                    s_best_move = move;
            }

            alpha = std::max(alpha, best_val);

            if (beta <= alpha)
                break;
        }
        return best_val;
    }
    else {
        float best_val = std::numeric_limits<float>::max();

        for (auto move : moves) {
            board new_board{b};
            new_board.make_move(move);

            float temp_val = minimax(new_board, depth + 1, alpha, beta, true);

            if (temp_val < best_val) {
                best_val = temp_val;

                if (depth == 0)
                    s_best_move = move;
            }

            beta = std::min(beta, best_val);

            if (beta <= alpha)
                break;
        }
        return best_val;
    }
}