#pragma once
#include <cstdint>
#include <iostream>
#include <ranges>
#include "engine/algorithms/eval.hpp"

#ifdef CPPCHESSENGINE_DEBUG
#include <array>
#endif

class board {

    friend struct eval;
    friend struct move_gen;
    friend struct engine;

public:

    board();
    explicit board(const std::string& str);

    ~board() = default;

    board(const board&) = default;
    board& operator=(const board&) = default;

    board(board&&) = default;
    board& operator=(board&&) = default;

    [[nodiscard]]
    uint64_t get_occupancy_board() const;
    [[nodiscard]]
    uint64_t get_white_occupancy_board() const;
    [[nodiscard]]
    uint64_t get_black_occupancy_board() const;

    void make_move(const uint32_t move);

#ifdef CPPCHESSENGINE_DEBUG

[[nodiscard]]
std::array<uint64_t, 6> get_white_bitboards() const;

[[nodiscard]]
std::array<uint64_t, 6> get_black_bitboards() const;

static void print_bitboard(const uint64_t bb);

void print_mailbox() const;

#endif

    enum PIECES {
        PAWN = 0,
        KNIGHT = 1,
        BISHOP = 2,
        ROOK = 3,
        QUEEN = 4,
        KING = 5,
    };

    enum RANKS : uint64_t {
        ONE     = 0x00000000000000FFULL,
        TWO     = 0x000000000000FF00ULL,
        THREE   = 0x0000000000FF0000ULL,
        FOUR    = 0x00000000FF000000ULL,
        FIVE    = 0x000000FF00000000ULL,
        SIX     = 0x0000FF0000000000ULL,
        SEVEN   = 0x00FF000000000000ULL,
        EIGHT   = 0xFF00000000000000ULL,
    };

    enum FILES : uint64_t {
        A = 0x0101010101010101ULL,
        B = 0x0202020202020202ULL,
        C = 0x0404040404040404ULL,
        D = 0x0808080808080808ULL,
        E = 0x1010101010101010ULL,
        F = 0x2020202020202020ULL,
        G = 0x4040404040404040ULL,
        H = 0x8080808080808080ULL,
    };

private:
    // There's probably a better way to arrange this for padding and cache locality
    std::array<uint64_t, 6> m_white = {0};    // PKBRQK
    std::array<uint64_t, 6> m_black = {0};    // PKBRQK
    bool m_white_turn;
    int m_castling;                 // bit shifted KQkq
    int m_enpassant;
    int m_halfmove_clock;
    int m_fullmove_clock;

    // Lookup for faster piece type on a square
    std::array<unsigned short, 64> m_mailbox;
    constexpr static unsigned short m_mailbox_black_flag{1 << 4}; // 4th bit used as colour flag since you can't differentiate 0 and -0 as white / black pawns


    enum castling_flags {
        white_king_side = 0b1000,
        white_queen_side = 0b0100,
        white_flags = 0b1100,
        black_king_side = 0b0010,
        black_queen_side = 0b0001,
        black_flags = 0b0011,
    };
    
};