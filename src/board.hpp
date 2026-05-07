#pragma once
#include <iostream>
#include <ranges>
#include "engine/algorithms/eval.hpp"

class board {

    friend struct eval;

public:

    board();
    explicit board(const std::string& str);

    ~board() = default;

    board(const board&) = default;
    board& operator=(const board&) = default;

    board(board&&) = default;
    board& operator=(board&&) = default;

#ifdef CPPCHESSENGINE_DEBUG

#include <array>
    [[nodiscard]]
    std::array<unsigned long long, 6> get_white_bitboards() const {
        return m_white;
    }

    [[nodiscard]]
    std::array<unsigned long long, 6> get_black_bitboards() const {
        return m_black;
    }

    static void print_bitboard(const unsigned long long bb) {
        for (int rank = 7; rank >= 0; --rank) {
            for (int file = 0; file < 8; ++file) {
                const int square = rank * 8 + file;
                std::cout << ((bb >> square) & 1ULL) << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
#endif

private:
    // There's probably a better way to arrange this for padding and cache locality
    std::array<unsigned long long, 6> m_white = {0};    // PKBRQK
    std::array<unsigned long long, 6> m_black = {0};    // PKBRQK
    bool m_white_turn;
    short m_castling;                 // bit shifted KQkq
    short m_enpassant;
    short m_halfmove_clock;
    short m_fullmove_clock;

    enum PIECES {
        PAWN = 0,
        KNIGHT = 1,
        BISHOP = 2,
        ROOK = 3,
        QUEEN = 4,
        KING = 5,
    };
};