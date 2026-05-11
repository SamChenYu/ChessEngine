#include "board.hpp"
#include <charconv>
#include <iomanip>
#include <vector>

board::board()
    : board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
{
}

board::board(const std::string& str) {

    std::vector<std::string_view> split;
    constexpr std::string_view delim = " ";
    for (const auto word : std::views::split(str, delim))
        split.emplace_back(word);

    if (split.size() < 6)
        throw std::invalid_argument("Invalid string length: less than 6");

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
                m_white[PAWN] |= 1ULL << current_square;
                break;
            case 'N':
                m_white[KNIGHT] |= 1ULL << current_square;
                break;
            case 'B':
                m_white[BISHOP] |= 1ULL << current_square;
                break;
            case 'R':
                m_white[ROOK] |= 1ULL << current_square;
                break;
            case 'Q':
                m_white[QUEEN] |= 1ULL << current_square;
                break;
            case 'K':
                m_white[KING] |= 1ULL << current_square;
                break;
            case 'p':
                m_black[PAWN] |= 1ULL << current_square;
                break;
            case 'n':
                m_black[KNIGHT] |= 1ULL << current_square;
                break;
            case 'b':
                m_black[BISHOP] |= 1ULL << current_square;
                break;
            case 'r':
                m_black[ROOK] |= 1ULL << current_square;
                break;
            case 'q':
                m_black[QUEEN] |= 1ULL << current_square;
                break;
            case 'k':
                m_black[KING] |= 1ULL << current_square;
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
        m_white_turn = true;
    else if (split[1] == "b")
        m_white_turn = false;
    else
        throw std::invalid_argument("Invalid FEN string: unrecognized turn character");

    m_castling = 0;
    while (!split[2].empty()) {
        switch (split[2][0]) {
            case 'K':
                if (m_castling >> 3 == 1)
                    throw std::invalid_argument("Invalid FEN string: castling has multiple 'K'");
                m_castling |= 1 << 3;
                break;
            case 'Q':
                if (m_castling >> 2 == 1)
                    throw std::invalid_argument("Invalid FEN string: castling has multiple 'Q'");
                m_castling |= 1 << 2;
                break;
            case 'k':
                if (m_castling >> 1 == 1)
                    throw std::invalid_argument("Invalid FEN string: castling has multiple 'k");
                m_castling |= 1 << 1;
                break;
            case 'q':
                if (m_castling == 1)
                    throw std::invalid_argument("Invalid FEN string: castling has multiple 'q'");
                m_castling |= 1 << 0;
                break;
            default:
                throw std::invalid_argument("Invalid FEN string: unrecognized castling character");
        }
        split[2].remove_prefix(1);
    }

    if (split[3][0] == '-') {
        m_enpassant = -1;
    } else {
        auto enpassant_result = std::from_chars(split[3].data(), split[3].data() + split[3].size(), m_enpassant);
        if (enpassant_result.ec == std::errc::invalid_argument) {
            throw std::invalid_argument("Invalid FEN string: invalid enpassant character");
        }
    }

    auto halfmove_result = std::from_chars(split[4].data(), split[4].data() + split[4].size(), m_halfmove_clock);
    if (halfmove_result.ec == std::errc::invalid_argument) {
        throw std::invalid_argument("Invalid FEN string: invalid half move clock character");
    }

    auto fullmove_result = std::from_chars(split[5].data(), split[5].data() + split[5].size(), m_fullmove_clock);
    if (fullmove_result.ec == std::errc::invalid_argument) {
        throw std::invalid_argument("Invalid FEN string: invalid full move clock character");
    }

}

uint64_t board::get_occupancy_board() const {
    uint64_t result{0};
    for (int i=0; i<6; i++) {
        result |= m_white[i];
        result |= m_black[i];
    }
    return result;
}

uint64_t board::get_white_occupancy_board() const {
    uint64_t result{0};
    for (int i=0; i<6; i++) {
        result |= m_white[i];
    }
    return result;
}

uint64_t board::get_black_occupancy_board() const {
    uint64_t result{0};
    for (int i=0; i<6; i++) {
        result |= m_black[i];
    }
    return result;
}