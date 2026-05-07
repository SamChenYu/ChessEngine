#pragma once

#include "../../board.hpp"
#include <vector>

class move {
public:
    move(int source, int target);
private:
    int m_source;
    int m_target;
};

struct move_gen {
public:
    static std::vector<move> generate_legal_moves(const board& board);
    static bool is_king_checked(const board& board);
};