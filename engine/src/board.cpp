#include <charconv>
#include <iomanip>
#include <limits>
#include <ranges>
#include <vector>

#include "board.hpp"
#include "algorithms/move_gen.hpp"
#include "config.hpp"

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
    
    if (split[3] == "-") {
        m_enpassant = -1;
    } else {
        if (split[3].size() != 2) {
            throw std::invalid_argument("Invalid FEN string: invalid en passant square");
        }

        char fileChar = split[3][0];
        char rankChar = split[3][1];

        if (fileChar < 'a' || fileChar > 'h' ||
            rankChar < '1' || rankChar > '8') {
            throw std::invalid_argument("Invalid FEN string: invalid en passant square");
            }

        int file = fileChar - 'a';
        int rank = rankChar - '1';

        m_enpassant = rank * 8 + file;
    }

    auto halfmove_result = std::from_chars(split[4].data(), split[4].data() + split[4].size(), m_halfmove_clock);
    if (halfmove_result.ec == std::errc::invalid_argument) {
        throw std::invalid_argument("Invalid FEN string: invalid half move clock character");
    }

    auto fullmove_result = std::from_chars(split[5].data(), split[5].data() + split[5].size(), m_fullmove_clock);
    if (fullmove_result.ec == std::errc::invalid_argument) {
        throw std::invalid_argument("Invalid FEN string: invalid full move clock character");
    }



    // Init the mailboxes
    /*
        OK this is quite poor tech debt
        the enum PIECES has pawn = 0 etc ... which means that for the mailbox we can't differentiate
        0 or -0 for white / black pawn. Instead I have chosen to use the 4th bit of the short as the colour flag
        This is also why an empty square is the short max value
     */
    m_mailbox.fill(std::numeric_limits<short>::max());
    for (int sq=0; sq<64; sq++) {
        const uint64_t curr_square{1ULL << sq};
        for (short i=0; i<6; i++) {
            if (m_white[i] & curr_square) {
                m_mailbox[sq] = i;
            } else if (m_black[i] & curr_square) {
                m_mailbox[sq] = (i | m_mailbox_black_flag);
            }
        }
    }

}

[[nodiscard]]
uint64_t board::get_occupancy_board() const {
    uint64_t result{0};
    for (int i=0; i<6; i++) {
        result |= m_white[i];
        result |= m_black[i];
    }
    return result;
}

[[nodiscard]]
uint64_t board::get_white_occupancy_board() const {
    uint64_t result{0};
    for (int i=0; i<6; i++) {
        result |= m_white[i];
    }
    return result;
}

[[nodiscard]]
uint64_t board::get_black_occupancy_board() const {
    uint64_t result{0};
    for (int i=0; i<6; i++) {
        result |= m_black[i];
    }
    return result;
}

[[nodiscard]]
std::array<uint64_t, 6> board::get_white_bitboards() const {
    return m_white;
}

[[nodiscard]]
std::array<uint64_t, 6> board::get_black_bitboards() const {
    return m_black;
}

void board::print_bitboard(const uint64_t bb) {
    for (int rank = 7; rank >= 0; --rank) {
        for (int file = 0; file < 8; ++file) {
            const int square = rank * 8 + file;
            std::cout << ((bb >> square) & 1ULL) << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

static_assert(sizeof(short) == 2);

void board::print_mailbox() const {
    for (int i=0; i<64; i++) {

        char fen_symbol;

        if (auto value = m_mailbox[i]; value == std::numeric_limits<short>::max()) {
            fen_symbol = '.';
        } else {
            // Todo: if I have any other uses this can be abstracted out into another function
            if (value & m_mailbox_black_flag) {
                value &= ~m_mailbox_black_flag; // remove the black flag
                switch (value) {
                    case 0:
                        fen_symbol = 'p';
                        break;
                    case 1:
                        fen_symbol = 'n';
                        break;
                    case 2:
                        fen_symbol = 'b';
                        break;
                    case 3:
                        fen_symbol = 'r';
                        break;
                    case 4:
                        fen_symbol = 'q';
                        break;
                    case 5:
                        fen_symbol = 'k';
                        break;
                    default:
                        fen_symbol = '!'; // Assume something went wrong
                }
            } else {
                switch (value) {
                    case 0:
                        fen_symbol = 'P';
                        break;
                    case 1:
                        fen_symbol = 'N';
                        break;
                    case 2:
                        fen_symbol = 'B';
                        break;
                    case 3:
                        fen_symbol = 'R';
                        break;
                    case 4:
                        fen_symbol = 'Q';
                        break;
                    case 5:
                        fen_symbol = 'K';
                        break;
                    default:
                        fen_symbol = '!'; // Assume something went wrong
                }
            }
        }
        std::cout << " " << fen_symbol << " ";

        if ((i+1)%8 == 0)
            std::cout << std::endl;
    }
}

void board::make_move(const uint32_t move) {

    std::array<uint64_t, 6>& friendly_pieces = m_white_turn ? m_white : m_black;

    const uint32_t from = move & move_gen::bitmask::from;
    const uint32_t to = (move & move_gen::bitmask::to) >> move_gen::bitmask::to_offset;
    const unsigned short from_piece_type = m_white_turn ? m_mailbox[from] : m_mailbox[from] & ~m_mailbox_black_flag;


    friendly_pieces[from_piece_type] &= ~(1ULL << from); // Remove the piece at the 'from' square
    m_mailbox[from] = std::numeric_limits<short>::max(); // Remove the piece from the mailbox

    // If king or rook move, then it needs to update the castling rights
    if (from_piece_type == PIECES::KING) {
        m_castling = m_white_turn ? m_castling | castling_flags::black_flags : m_castling | castling_flags::white_flags;
    }
    if (from_piece_type == PIECES::ROOK) {
        if (m_white_turn) {
            if (from == 0)
                m_castling &= ~white_queen_side;
            else if (from == 7)
                m_castling &= ~white_king_side;
        } else {
            if (from == 56)
                m_castling &= ~black_queen_side;
            else if (from == 63)
                m_castling &= ~black_king_side;
        }
    }

    if (const uint32_t promotion = (move & move_gen::bitmask::promotion) >> move_gen::bitmask::promotion_offset) {
        switch (promotion) {
            case 1:
                friendly_pieces[PIECES::KNIGHT] |= (1ULL << to);
                m_mailbox[to] = m_white_turn ? PIECES::KNIGHT : PIECES::KNIGHT | m_mailbox_black_flag;
                break;
            case 2:
                friendly_pieces[PIECES::BISHOP] |= (1ULL << to);
                m_mailbox[to] = m_white_turn? PIECES::BISHOP : PIECES::BISHOP | m_mailbox_black_flag;
                break;
            case 3:
                friendly_pieces[PIECES::ROOK] |= (1ULL << to);
                m_mailbox[to] = m_white_turn ? PIECES::ROOK : PIECES::ROOK | m_mailbox_black_flag;
                break;
            case 4:
                friendly_pieces[PIECES::QUEEN] |= (1ULL << to);
                m_mailbox[to] = m_white_turn? PIECES::QUEEN : PIECES::QUEEN | m_mailbox_black_flag;
                break;
            default:
                throw std::runtime_error("board::make_move promotion branch unrecognised promotion bitmask");
        }
    } else {
        friendly_pieces[from_piece_type] |= (1ULL << to); // Add the piece at the 'to' square (not a promotion)
        m_mailbox[to] = from_piece_type;
    }

    const bool capture = (move & move_gen::bitmask::capture) >> move_gen::bitmask::capture_offset;
    if (capture) {
        // Remove the enemy piece
        std::array<uint64_t, 6>& enemy_pieces = m_white_turn ? m_white : m_black;
        const unsigned short to_piece_type = m_mailbox[to];
        enemy_pieces[to_piece_type] &= ~(1ULL << to);
    }

    if (const bool enpassant = (move & move_gen::bitmask::enpassant) >> move_gen::bitmask::enpassant_offset) {
        // Remove the enemy pawn
        std::array<uint64_t, 6>& enemy_pieces = m_white_turn ? m_white : m_black;
        // Todo: remove the enemy pawn
    }

    if (const uint32_t castling = (move & move_gen::bitmask::castling) >> move_gen::bitmask::castling_offset) {
        // Todo bit mask for each case
    }

    // Half move clock: reset to 0 if pawn move or not a capture move. Otherwise reset to 0
    m_halfmove_clock = (from_piece_type != PIECES::PAWN && !capture) ? m_halfmove_clock++ : 0;
    m_fullmove_clock++;

}