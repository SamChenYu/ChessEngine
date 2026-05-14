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

    enum bitmask : uint32_t {
        from        =       0b00000000000000000000000000111111, from_offset = 0,
        to          =       0b00000000000000000000111111000000, to_offset = 6,
        capture     =       0b00000000000000000001000000000000, capture_offset = 12,
        enpassant   =       0b00000000000000000010000000000000, enpassant_offset = 13,
        castling    =       0b00000000000000001100000000000000, castling_offset = 14,
        promotion   =       0b00000000000011110000000000000000, promotion_offset = 16
    };

    constexpr static std::array<uint64_t, 64> generate_knight_table() {
        std::array<uint64_t, 64> table{};

        for (int sq = 0; sq < 64; ++sq) {
            uint64_t b = 1ULL << sq;

            uint64_t knight_attacks = 0;

            knight_attacks |= (b << 17) & ~board::FILES::A;
            knight_attacks |= (b << 15) & ~board::FILES::H;
            knight_attacks |= (b << 10) & ~(board::FILES::A | board::FILES::B);
            knight_attacks |= (b << 6)  & ~(board::FILES::G | board::FILES::H);

            knight_attacks |= (b >> 17) & ~board::FILES::H;
            knight_attacks |= (b >> 15) & ~board::FILES::A;
            knight_attacks |= (b >> 10) & ~(board::FILES::G | board::FILES::H);
            knight_attacks |= (b >> 6)  & ~(board::FILES::A | board::FILES::B);

            table[sq] = knight_attacks;
        }

        return table;
    }

    constexpr static std::array<uint64_t, 64> generate_king_table() {
        std::array<uint64_t, 64> table{};

        for (int sq = 0; sq < 64; ++sq) {
            uint64_t b = 1ULL << sq;
            uint64_t king_attacks = 0;
            // north
            king_attacks |= b << 8;
            // south
            king_attacks |= b >> 8;
            // east / west masks to prevent wrap
            king_attacks |= (b << 1) & ~board::FILES::A;
            king_attacks |= (b >> 1) & ~board::FILES::H;
            // diagonals
            king_attacks |= (b << 9) & ~board::FILES::A;
            king_attacks |= (b << 7) & ~board::FILES::H;
            king_attacks |= (b >> 7) & ~board::FILES::A;
            king_attacks |= (b >> 9) & ~board::FILES::H;

            table[sq] = king_attacks;
        }

        return table;
    }

#ifdef CPPCHESSENGINE_DEBUG
    static void print_move(uint32_t move) {
        uint32_t from = move & bitmask::from;
        uint32_t to = (move & bitmask::to) >> bitmask::to_offset;
        uint32_t capture = (move & bitmask::capture) >> bitmask::capture_offset;
        uint32_t enpassant = (move & bitmask::enpassant) >> bitmask::enpassant_offset;
        uint32_t castling = (move & bitmask::castling) >> bitmask::castling_offset;
        uint32_t promotion = (move & bitmask::promotion) >> bitmask::promotion_offset;
        std::cout << "from:" << " " << from << " to:" << " " << to << " capture: " << capture << " enpasssant: " << enpassant << " castling: " << castling << " promotion: " << promotion << std::endl;
    }
#endif

};