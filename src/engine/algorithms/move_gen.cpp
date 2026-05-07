#include "move_gen.h"


move::move(int source, int target, int flags, int extra) : m_source{source}, m_target{target}, m_extra{extra}, m_flags{flags} {
}


std::vector<move> move_gen::generate_legal_moves(const board &board) {
    std::vector<move> legal_moves;

    return legal_moves;
}

bool move_gen::is_king_checked(const board &board) {
    return true;
}
