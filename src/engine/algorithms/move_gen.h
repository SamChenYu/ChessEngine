#pragma once

#include "../../board.hpp"
#include <vector>

class move {
public:
    move(int source, int target, int flags, int extra);

    // Todo: just make a constructors default

    enum MOVEFLAGS {
        CAPTURE       = 1 << 0,
        CASTLE        = 1 << 1,
        EN_PASSANT    = 1 << 2,
        DOUBLE_PUSH   = 1 << 3
    };


private:
    // Todo: stockfish style move with just 32 bits!!!!
    int m_source;
    int m_target;
    int m_flags;
    int m_extra;
};

struct move_gen {
public:
    static std::vector<move> generate_legal_moves(const board& board);
    static bool is_king_checked(const board& board);
};