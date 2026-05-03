#include "board.hpp"

#include <charconv>
#include <iomanip>
#include <iostream>

board::board() : white_turn{true}, castling{15},
                 enpassant{0}, halfmove_clock{0}, fullmove_clock{0} {
    // init the boards themselves
}

board::board(const std::string str) {

    std::vector<std::string_view> split;
    constexpr std::string_view delim = " ";
    for (const auto word : std::views::split(str, delim))
        split.emplace_back(word);

    if (split.size() < 6)
        throw std::invalid_argument("Invalid string length: less than 6");

    // Starting FEN Position:
    //          rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1

    // deal with split[0]

    if (split[1] == "w")
        white_turn = true;
    else if (split[1] == "b")
        white_turn = false;
    else
        throw std::invalid_argument("Invalid FEN string: unrecognized turn character");

    castling = 0;
    while (!split[2].empty()) {
        switch (split[2][0]) {
            case 'K':
                if (castling >> 3 == 1)
                    throw std::invalid_argument("Invalid FEN string: castling has multiple 'K'");
                castling += 1 << 3;
                break;
            case 'Q':
                if (castling >> 2 == 1)
                    throw std::invalid_argument("Invalid FEN string: castling has multiple 'Q'");
                castling += 1 << 2;
                break;
            case 'k':
                if (castling >> 1 == 1)
                    throw std::invalid_argument("Invalid FEN string: castling has multiple 'k");
                castling += 1 << 1;
                break;
            case 'q':
                if (castling == 1)
                    throw std::invalid_argument("Invalid FEN string: castling has multiple 'K'");
                castling += 1 << 0;
                break;
            default:
                throw std::invalid_argument("Invalid FEN string: unrecognized castling character: "  + split[2][0]);
        }
        split[2].remove_prefix(1);
    }

    if (split[3][0] == '-') {
        enpassant = -1;
    } else {
        auto enpassant_result = std::from_chars(split[3].data(), split[3].data() + split[3].size(), enpassant);
        if (enpassant_result.ec == std::errc::invalid_argument) {
            throw std::invalid_argument("Invalid FEN string: invalid enpassant character");
        }
    }

    auto halfmove_result = std::from_chars(split[4].data(), split[4].data() + split[4].size(), halfmove_clock);
    if (halfmove_result.ec == std::errc::invalid_argument) {
        throw std::invalid_argument("Invalid FEN string: invalid half move clock character");
    }

    auto fullmove_result = std::from_chars(split[5].data(), split[5].data() + split[5].size(), fullmove_clock);
    if (fullmove_result.ec == std::errc::invalid_argument) {
        throw std::invalid_argument("Invalid FEN string: invalid full move clock character");
    }

    std::cout << std::boolalpha;
}