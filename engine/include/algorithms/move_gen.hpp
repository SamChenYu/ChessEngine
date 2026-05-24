#pragma once

#include <vector>
#include "magic_bits.hpp"
#include "board.hpp"

struct move_gen {
public:
    // General logic: compute pseudo moves, only check legality whilst making them in the search function
    static void generate_pseudo_moves(const board& board, std::vector<uint32_t>& out);
    static bool is_legal(const board& board);
    static uint64_t generate_enemy_attack_bitboard(const board& board);
    static uint64_t generate_friendly_attack_bitboard(const board& board);
    /*
        Moves are encoded into 32 bit integers
        // General search is going to be made by copying board states,
        // not by moving & unmoving so we will disregard DOUBLE PAWN PUSH
    */
    enum bitmask : uint32_t {
        from        =       0b00000000000000000000000000111111, from_offset = 0,        // Interpreted as 6 bit integer (0-63)
        to          =       0b00000000000000000000111111000000, to_offset = 6,          // Interpreted as 6 bit integer (0-63)
        capture     =       0b00000000000000000001000000000000, capture_offset = 12,    // Interpreted as a flag
        enpassant   =       0b00000000000000000010000000000000, enpassant_offset = 13,  // Interpreted as a flag
        castling    =       0b00000000000000001100000000000000, castling_offset = 14,   // Interpreted as 01(king side), 10 (queen side)
        promotion   =       0b00000000000011110000000000000000, promotion_offset = 16,   // Interpreted as 0001 (knight), 0010 (bishop), 0100 (rook) 1000 (queen)
    };

    // movegen_debug
    static void print_move(uint32_t move) {
        uint32_t from = move & bitmask::from;
        uint32_t to = (move & bitmask::to) >> bitmask::to_offset;
        uint32_t capture = (move & bitmask::capture) >> bitmask::capture_offset;
        uint32_t enpassant = (move & bitmask::enpassant) >> bitmask::enpassant_offset;
        uint32_t castling = (move & bitmask::castling) >> bitmask::castling_offset;
        uint32_t promotion = (move & bitmask::promotion) >> bitmask::promotion_offset;
        std::cout << "from:" << " " << from << " to:" << " " << to << " capture: " << capture << " enpasssant: " << enpassant << " castling: " << castling << " promotion: " << promotion << std::endl;
    }

private:
    inline static magic_bits::Attacks attacks;

    enum castling_bitmask : uint64_t {
        white_kingside_clear = (1ULL << 5) | (1ULL << 6),
        white_queenside_clear = (1ULL << 1) | (1ULL << 2) | (1ULL << 3),
        white_king = (1ULL << 4),
        white_kingside_rook = (1ULL << 7),
        white_queenside_rook = (1ULL << 0),

        black_kingside_clear = (1ULL << 61) | (1ULL << 62),
        black_queenside_clear = (1ULL << 57) | (1ULL << 58) | (1ULL << 59),
        black_king = (1ULL << 60),
        black_kingside_rook = (1ULL << 63),
        black_queenside_rook = (1ULL << 56),
    };

    inline constexpr static std::array<uint64_t, 64> generate_knight_table() {
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

    inline constexpr static std::array<uint64_t, 64> generate_king_table() {
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
};