#define CPPCHESSENGINE_DEBUG
#include "board.hpp"

#include <charconv>
#include <iomanip>
#include <vector>

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


    // rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR
    // a8....h8...........................a1....h1

    /*
       Chess Board (ranks/files)        Bitboard Indices

       8  a8 b8 c8 d8 e8 f8 g8 h8      [56][57][58][59][60][61][62][63]
       7  a7 b7 c7 d7 e7 f7 g7 h7      [48][49][50][51][52][53][54][55]
       6  a6 b6 c6 d6 e6 f6 g6 h6      [40][41][42][43][44][45][46][47]
       5  a5 b5 c5 d5 e5 f5 g5 h5      [32][33][34][35][36][37][38][39]
       4  a4 b4 c4 d4 e4 f4 g4 h4      [24][25][26][27][28][29][30][31]
       3  a3 b3 c3 d3 e3 f3 g3 h3      [16][17][18][19][20][21][22][23]
       2  a2 b2 c2 d2 e2 f2 g2 h2      [ 8][ 9][10][11][12][13][14][15]
       1  a1 b1 c1 d1 e1 f1 g1 h1      [ 0][ 1][ 2][ 3][ 4][ 5][ 6][ 7]

          a  b  c  d  e  f  g  h
    */


    int current_square{56};
    while (!split[0].empty()) {

        switch (split[0][0]) {
            case 'P':
                white[PAWN] |= 1ULL << current_square;
                break;
            case 'N':
                white[KNIGHT] |= 1ULL << current_square;
                break;
            case 'B':
                white[BISHOP] |= 1ULL << current_square;
                break;
            case 'R':
                white[ROOK] |= 1ULL << current_square;
                break;
            case 'Q':
                white[QUEEN] |= 1ULL << current_square;
                break;
            case 'K':
                white[KING] |= 1ULL << current_square;
                break;
            case 'p':
                black[PAWN] |= 1ULL << current_square;
                break;
            case 'n':
                black[KNIGHT] |= 1ULL << current_square;
                break;
            case 'b':
                black[BISHOP] |= 1ULL << current_square;
                break;
            case 'r':
                black[ROOK] |= 1ULL << current_square;
                break;
            case 'q':
                black[QUEEN] |= 1ULL << current_square;
                break;
            case 'k':
                black[KING] |= 1ULL << current_square;
                break;
            case '/':
                current_square -= 16;
                split[0].remove_prefix(1);
                continue;
            default:
                int empty_squares;
                auto result = std::from_chars(split[0].data(), split[0].data()+1, empty_squares);
                if (result.ec == std::errc::invalid_argument || empty_squares > 8 || empty_squares < 0) {
                    throw std::invalid_argument("Invalid FEN string: invalid character in pieces");
                }
                current_square += empty_squares;
                split[0].remove_prefix(1);
                continue;

        }
        current_square++;
        split[0].remove_prefix(1);
    }


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
                castling |= 1 << 3;
                break;
            case 'Q':
                if (castling >> 2 == 1)
                    throw std::invalid_argument("Invalid FEN string: castling has multiple 'Q'");
                castling |= 1 << 2;
                break;
            case 'k':
                if (castling >> 1 == 1)
                    throw std::invalid_argument("Invalid FEN string: castling has multiple 'k");
                castling |= 1 << 1;
                break;
            case 'q':
                if (castling == 1)
                    throw std::invalid_argument("Invalid FEN string: castling has multiple 'K'");
                castling |= 1 << 0;
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

}