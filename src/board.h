#pragma once
#include <ranges>
#include <string_view>
#include <vector>

class board {
public:

    board();
    explicit board(std::string_view);

    ~board() = default;

    board(const board&) = default;
    board& operator=(const board&) = default;

    board(board&&) = default;
    board& operator=(board&&) = default;

    static bool valid_fen(std::string_view str) {
        if (str.length() < 2) return false;

        


        return true;
    }



private:
    // There's probably a better way to arrange this for padding and cache locality
    unsigned long long white[6];    // PKBRQK
    unsigned long long black[6];    // PKBRQK
    bool white_turn;
    short castling;                 // bit shifted KQkq
    short enpassant;
    short halfmove_clock;
    short fullmove_clock;
};