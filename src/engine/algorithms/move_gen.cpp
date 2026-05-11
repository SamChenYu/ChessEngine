#define CPPCHESSENGINE_DEBUG

#include "move_gen.hpp"
#include "../../board.hpp"
#include "../../../external/magic-bits/include/magic_bits.hpp"
#include <vector>

void move_gen::generate_pseudo_moves(const board &b, std::vector<int>& out) {
    const uint64_t occupancy_bb{b.get_occupancy_board()};
    const std::array<uint64_t, 6>& friendly_pieces = b.m_white_turn ? b.m_white : b.m_black;

    uint64_t pawns = friendly_pieces[b.PIECES::PAWN];
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

    uint64_t knights = friendly_pieces[b.PIECES::KNIGHT];
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


    uint64_t bishops = friendly_pieces[b.PIECES::BISHOP];
    while (bishops) {
        int next_bishop = std::countr_zero(bishops);
        const uint64_t bishop_moves = attacks.Bishop(occupancy_bb, next_bishop); // Todo: exclude friendly piece captures
#ifdef CPPCHESSENGINE_DEBUG
        std::cout << __FILE__ << ":" << __LINE__ << " (" << __func__ << ") "  << "\n";
        std::cout << "bishops moves: " << std::endl;
        b.print_bitboard(bishop_moves);
#endif
        bishops &= bishops - 1;
    }


    uint64_t rooks = friendly_pieces[b.PIECES::ROOK];
    while (rooks) {
        int next_rook = std::countr_zero(rooks);
        const uint64_t rook_moves = attacks.Rook(occupancy_bb, next_rook); // Todo: exclude friendly piece captures
#ifdef CPPCHESSENGINE_DEBUG
        std::cout << __FILE__ << ":" << __LINE__ << " (" << __func__ << ") "  << "\n";
        std::cout << "Rook moves: " << std::endl;
        b.print_bitboard(rook_moves);
#endif
        rooks &= rooks - 1;
    }

    uint64_t queens = friendly_pieces[b.PIECES::QUEEN];
    while (queens) {
        int next_queen = std::countr_zero(queens);
        const uint64_t queen_moves = attacks.Queen(occupancy_bb, next_queen); // Todo: exclude friendly piece captures
#ifdef CPPCHESSENGINE_DEBUG
#include <iostream>
        std::cout << __FILE__ << ":" << __LINE__ << " (" << __func__ << ") "  << "\n";
        std::cout << "Queen moves: " << std::endl;
        b.print_bitboard(queen_moves);
#endif
        queens &= queens - 1;
    }

    uint64_t kings = friendly_pieces[b.PIECES::KING];
    // To be honest, for non-legal positions I can implment multiple kings but just gonna do one for now
    // Todo: implement Kings bitmask
    if (kings) {
        int next_king = std::countr_zero(kings);
#ifdef CPPCHESSENGINE_DEBUG
        std::cout << __FILE__ << ":" << __LINE__ << " (" << __func__ << ") "  << "\n";
        std::cout << "King moves: " << std::endl;
        // b.print_bitboard(king_moves);
#endif
    }


}


void move_gen::generate_legal_moves(const board& b, std::vector<int>& out) {
    generate_pseudo_moves(b, out);
}

bool move_gen::is_king_checked(const board &board) {
    return true;
}
