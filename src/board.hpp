#pragma once
#include <ranges>
#include <string_view>
#include <iostream>

class board {
public:

    board();
    explicit board(const std::string str);

    ~board() = default;

    board(const board&) = default;
    board& operator=(const board&) = default;

    board(board&&) = default;
    board& operator=(board&&) = default;

#ifdef CPPCHESSENGINE_DEBUG

#include <array>

    std::array<unsigned long long, 6> get_white_bitboards() const {
        return std::to_array(white);
    }

    std::array<unsigned long long, 6> get_black_bitboards() const {
        return std::to_array(black);
    }


    void print_bitboard(unsigned long long bb) {
        for (int rank = 7; rank >= 0; --rank) {
            for (int file = 0; file < 8; ++file) {
                int square = rank * 8 + file;
                std::cout << ((bb >> square) & 1ULL) << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
#endif

private:
    // There's probably a better way to arrange this for padding and cache locality
    unsigned long long white[6] = {};    // PKBRQK
    unsigned long long black[6] = {};    // PKBRQK
    bool white_turn;
    short castling;                 // bit shifted KQkq
    short enpassant;
    short halfmove_clock;
    short fullmove_clock;

    enum PIECES {
        PAWN = 0,
        KNIGHT = 1,
        BISHOP = 2,
        ROOK = 3,
        QUEEN = 4,
        KING = 5,
    };
};