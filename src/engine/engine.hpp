#pragma once

#include "../board.hpp"


class engine {

public:
    [[nodiscard]]
    static uint32_t start_minimax(const board& b, int max_depth);

    inline static uint32_t s_best_move;
    inline static float    s_best_move_eval;
private:
    inline static int s_max_depth;
    [[nodiscard]]
    static float minimax(const board& b, int depth, float alpha, float beta, bool is_maximising);
#ifdef CPPCHESSENGINE_DEBUG
    inline static int s_nodes_searched;
#endif
};