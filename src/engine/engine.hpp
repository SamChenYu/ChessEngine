#pragma once

#include "../board.hpp"


class engine {
public:
    static int max_depth;
    static float minimax(const board& b, int depth, float alpha, float beta, bool is_maximising);
};