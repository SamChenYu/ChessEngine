#pragma once

#include "../../board.hpp"
#include "../../../external/magic-bits/include/magic_bits.hpp"
#include <vector>

/*
    Moves are encoded into 32 bit integers
    bits 0 – 5   FROM
    bits 6 – 11  TO
    bits 12     CAPTURE FLAG
    bits 13     EN PASSANT FLAG
    bits 14 - 15     CASTLE FLAG
        00 = No Castling
        01 = Kingside
        10 = Queenside
    bits 16 – 19 PROMOTION PIECE
        000 = No Promotion
        001 = Knight
        010 = Bishop
        011 = Rook
        100 = QUEEN

    // General search is going to be made by copying board states,
    // not by moving / unmoving so we will disregard CAPTURE, and
    // DOUBLE PAWN PUSH


*/

struct move_gen {
public:
    static void generate_legal_moves(const board& board, std::vector<int>& out);
    static bool is_king_checked(const board& board);
private:
    static magic_bits::Attacks attacks;
};