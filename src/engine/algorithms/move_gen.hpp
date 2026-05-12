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

    static void generate_legal_moves(const board& board, std::vector<uint32_t>& out);
    static bool is_king_checked(const board& board);
private:
    static void generate_pseudo_moves(const board& board, std::vector<uint32_t>& out);
    inline static magic_bits::Attacks attacks;

    enum bitmask {
        from        =       0b00000000000000000000000000111111, from_offset = 0,
        to          =       0b00000000000000000000111111000000, to_offset = 6,
        capture     =       0b00000000000000000001000000000000, capture_offset = 12,
        enpassant   =       0b00000000000000000010000000000000, enpassant_offset = 13,
        castling    =       0b00000000000000001100000000000000, castling_offset = 14,
        promotion   =       0b00000000000011110000000000000000, promotion_offset = 16
        // from= 0x3F,
        // to = 0xFC0,
        // capture = 0x800,
        // enpassant = 0x1000,
        // castling = 0x6000,
        // promotion = 0x38000
    };

#ifdef CPPCHESSENGINE_DEBUG
    static void print_move(uint32_t move) {
        uint32_t from = move & bitmask::from;
        uint32_t to = (move & bitmask::to) >> bitmask::to_offset;
        std::cout << "from:" << " " << from << " to:" << " " << to << std::endl;
    }
#endif

};