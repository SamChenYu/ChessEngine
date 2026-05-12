#define CPPCHESSENGINE_DEBUG

#include "move_gen.hpp"
#include "../../board.hpp"
#include "../../../external/magic-bits/include/magic_bits.hpp"
#include <vector>

void move_gen::generate_pseudo_moves(const board &b, std::vector<uint32_t>& out) {

    const uint64_t occupancy_bb{b.get_occupancy_board()};
    const std::array<uint64_t, 6>& friendly_pieces = b.m_white_turn ? b.m_white : b.m_black;
    const std::array<uint64_t, 6>& enemy_pieces = b.m_white_turn? b.m_black : b.m_white;

    uint64_t friendly_occupancy_bb{0};
    uint64_t enemy_occupancy_bb{0};
    for (int i=0; i<6; i++) {
        friendly_occupancy_bb |= friendly_pieces[i];
        enemy_occupancy_bb |= enemy_pieces[i];
    }

    uint64_t pawns = friendly_pieces[board::PAWN];
    while (pawns) {
        int next_pawns = std::countr_zero(pawns);
        // Todo: implement pawns bitmask
#ifdef CPPCHESSENGINE_DEBUG
#include <iostream>
        std::cout << __FILE__ << ":" << __LINE__ << " (" << __func__ << ") "  << "\n";
        std::cout << "Pawn moves: " << std::endl;
        //b.print_bitboard(pawn_moves);
#endif
        pawns &= pawns - 1;
    }

    uint64_t knights = friendly_pieces[board::KNIGHT];
    while (knights) {
        int next_knights = std::countr_zero(knights);
        // Todo: implement knights bitmask
#ifdef CPPCHESSENGINE_DEBUG
        std::cout << __FILE__ << ":" << __LINE__ << " (" << __func__ << ") "  << "\n";
        std::cout << "Knight moves: " << std::endl;
        //b.print_bitboard(knight_moves);
#endif
        knights &= knights - 1;
    }


    uint64_t bishops = friendly_pieces[board::BISHOP];
    while (bishops) {
        int next_bishop = std::countr_zero(bishops);
        uint64_t bishop_moves = attacks.Bishop(occupancy_bb, next_bishop);
        bishop_moves = bishop_moves & ~friendly_occupancy_bb;
#ifdef CPPCHESSENGINE_DEBUG
        std::cout << __FILE__ << ":" << __LINE__ << " (" << __func__ << ") "  << "\n";
        std::cout << "Bishops moves: " << std::endl;
        b.print_bitboard(bishop_moves);
#endif
        bishops &= bishops - 1;
    }


    uint64_t rooks = friendly_pieces[board::ROOK];
    while (rooks) {
        int next_rook = std::countr_zero(rooks);
        uint64_t rook_moves = attacks.Rook(occupancy_bb, next_rook);
        rook_moves = rook_moves & ~friendly_occupancy_bb;

#ifdef CPPCHESSENGINE_DEBUG
        std::cout << __FILE__ << ":" << __LINE__ << " (" << __func__ << ") "  << "\n";
        std::cout << "Rook moves: " << std::endl;
        b.print_bitboard(rook_moves);
#endif

        uint64_t quiet_moves = rook_moves & ~occupancy_bb;
        uint64_t captures = rook_moves & enemy_occupancy_bb;

        while (quiet_moves) {
            int move{0};
            move = (next_rook & bitmask::from);
            move = move |= ((std::countr_zero(quiet_moves) << bitmask::to_offset) & bitmask::to);
            out.emplace_back(move);
            quiet_moves &= quiet_moves -1;
        }

        while (captures) {
            int move{0};
            move = (next_rook & bitmask::from);
            move = move |= ((std::countr_zero(captures) << bitmask::to_offset) & bitmask::to);
            move = move |= 0b1 << bitmask::capture_offset & bitmask::capture;
            out.emplace_back(move);
            captures &= captures - 1;
        }


        rooks &= rooks - 1;
    }

    uint64_t queens = friendly_pieces[board::QUEEN];
    while (queens) {
        int next_queen = std::countr_zero(queens);
        uint64_t queen_moves = attacks.Queen(occupancy_bb, next_queen);
        queen_moves = queen_moves & ~friendly_occupancy_bb;
#ifdef CPPCHESSENGINE_DEBUG
#include <iostream>
        std::cout << __FILE__ << ":" << __LINE__ << " (" << __func__ << ") "  << "\n";
        std::cout << "Queen moves: " << std::endl;
        b.print_bitboard(queen_moves);
#endif
        queens &= queens - 1;
    }

    uint64_t kings = friendly_pieces[board::KING];
    // To be honest, for non-legal positions I can implment multiple kings but just gonna do one for now
    // Todo: implement Kings bitmask
    if (kings) {
        int next_king = std::countr_zero(kings);
#ifdef CPPCHESSENGINE_DEBUG
        std::cout << __FILE__ << ":" << __LINE__ << " (" << __func__ << ") "  << "\n";
        std::cout << "King moves: " << std::endl;
        // b.print_bitboard(king_moves);
#endif

#ifdef CPPCHESSENGINE_DEBUG
        std::cout << "Moves: " << std::endl;
for (auto move : out) {
    print_move(move);
}
#endif
    }


}


void move_gen::generate_legal_moves(const board& b, std::vector<uint32_t>& out) {
    generate_pseudo_moves(b, out);
}

bool move_gen::is_king_checked(const board &board) {
    return true;
}
