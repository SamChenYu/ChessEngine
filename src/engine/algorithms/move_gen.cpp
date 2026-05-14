#define CPPCHESSENGINE_DEBUG

#include "move_gen.hpp"
#include "../../board.hpp"
#include "../../../external/magic-bits/include/magic_bits.hpp"
#include <vector>

#ifdef CPPCHESSENGINE_DEBUG
#include <iostream>
#endif

void move_gen::generate_pseudo_moves(const board &b, std::vector<uint32_t>& out) {

    out.reserve(100);
    const uint64_t occupancy_bb{b.get_occupancy_board()};
    const std::array<uint64_t, 6>& friendly_pieces = b.m_white_turn ? b.m_white : b.m_black;
    const std::array<uint64_t, 6>& enemy_pieces = b.m_white_turn? b.m_black : b.m_white;

    constexpr uint32_t capture_flag = (0b1 << bitmask::capture_offset) & bitmask::capture;


    uint64_t friendly_occupancy_bb{0};
    uint64_t enemy_occupancy_bb{0};
    for (int i=0; i<6; i++) {
        friendly_occupancy_bb |= friendly_pieces[i];
        enemy_occupancy_bb |= enemy_pieces[i];
    }


    // PAWNS FIRST
    // We are branching off for white / black because bit shifting by negative numbers is UB

    if (b.m_white_turn) {
        uint64_t pawns = friendly_pieces[board::PAWN];
        // Move forward = >> or << by 8 then &= with ~occupancy_bb
        //      Also can &= with the final rank for promotion

        // white
        uint64_t pawn_single_step{ (pawns << 8) & ~occupancy_bb & ~board::RANKS::EIGHT}; // March forward except promotion rank
        uint64_t pawn_double_step{ ((pawn_single_step  & board::RANKS::THREE) << 8) & ~occupancy_bb}; // Quickly get the value before we reduce single_step
        while (pawn_single_step) {
            uint32_t to = std::countr_zero(pawn_single_step);
            uint32_t move{to - 8};
            move |= (to << bitmask::to_offset) & bitmask::to;
            out.emplace_back(move);
            pawn_single_step &= pawn_single_step - 1;
        }
        while (pawn_double_step) {
            uint32_t to = std::countr_zero(pawn_double_step);
            uint32_t move{to - 16};
            move |= (to << bitmask::to_offset) & bitmask::to;
            out.emplace_back(move);
            pawn_double_step &= pawn_double_step - 1;
        }

        uint64_t pawn_promotion{ (pawns << 8) & ~occupancy_bb & board::RANKS::EIGHT}; // only Promotion
        while (pawn_promotion) {
            uint32_t to = std::countr_zero(pawn_promotion);
            for (int i=1; i<5; i++) { // Iterate for KNIGHT / BISHOP / ROOK / QUEEN Promotions
                uint32_t move{to - 8};
                move |= (to << bitmask::to_offset) & bitmask::to;
                move |= (i << bitmask::promotion_offset) & bitmask::promotion;
                out.emplace_back(move);
            }
            pawn_promotion &= pawn_promotion - 1;
        }

        // Captures = >> or << by 7 and 9 and then &= with enemy_occupancy_bb
        // Check for en passant captures

        uint64_t right_captures{ ((pawns & ~board::FILES::H) << 9) & enemy_occupancy_bb & ~board::RANKS::EIGHT};

        while (right_captures) {
            uint32_t to = std::countr_zero(right_captures);
            uint32_t move{to - 9};
            move |= (to << bitmask::to_offset) & bitmask::to;
            move |= capture_flag;
            out.emplace_back(move);
            right_captures &= right_captures - 1;
        }
        uint64_t left_captures{ ((pawns & ~board::FILES::A) << 7) & enemy_occupancy_bb & ~board::RANKS::EIGHT};
        while (left_captures) {
            uint32_t to = std::countr_zero(left_captures);
            uint32_t move{to - 7};
            move |= (to << bitmask::to_offset) & bitmask::to;
            move |= capture_flag;
            out.emplace_back(move);
            left_captures &= left_captures - 1;
        }

        const uint64_t en_passant_square = (b.m_enpassant >= 0) ? (1ULL << b.m_enpassant) : 0ULL;
        uint64_t right_enpassant_captures{ ((pawns & ~board::FILES::H) << 9) & en_passant_square };
        while (right_enpassant_captures) {
            uint32_t to = std::countr_zero(right_enpassant_captures);
            uint32_t move{to - 9};
            move |= (to << bitmask::to_offset) & bitmask::to;
            move |= capture_flag;
            move |= (0b1 << bitmask::enpassant_offset) & bitmask::enpassant;
            out.emplace_back(move);
            right_enpassant_captures &= right_enpassant_captures - 1;
        }

        uint64_t left_enpassant_captures{ ((pawns & ~board::FILES::A) << 7) & en_passant_square };
        while (left_enpassant_captures) {
            uint32_t to = std::countr_zero(left_enpassant_captures);
            uint32_t move{to - 7};
            move |= (to << bitmask::to_offset) & bitmask::to;
            move |= capture_flag;
            move |= (0b1 << bitmask::enpassant_offset) & bitmask::enpassant;
            out.emplace_back(move);
            left_enpassant_captures &= left_enpassant_captures - 1;
        }

        uint64_t right_capture_promotion{ ((pawns & ~board::FILES::H) << 9) & enemy_occupancy_bb & board::RANKS::EIGHT};
        while (right_capture_promotion) {
            uint32_t to = std::countr_zero(right_capture_promotion);
            for (int i=1; i<5; i++) {
                // Iterate for KNIGHT / BISHOP / ROOK / QUEEN Promotions
                uint32_t move{to - 9};
                move |= (to << bitmask::to_offset) & bitmask::to;
                move |= capture_flag;
                move |= (i << bitmask::promotion_offset) & bitmask::promotion;
                out.emplace_back(move);
            }
            right_capture_promotion &= right_capture_promotion - 1;
        }

        uint64_t left_capture_promotion{ ((pawns & ~board::FILES::A) << 7) & enemy_occupancy_bb & board::RANKS::EIGHT};
        while (left_capture_promotion) {
            uint32_t to = std::countr_zero(left_capture_promotion);
            for (int i=1; i<5; i++) {
                // Iterate for KNIGHT / BISHOP / ROOK / QUEEN Promotions
                uint32_t move{to - 7};
                move |= (to << bitmask::to_offset) & bitmask::to;
                move |= capture_flag;
                move |= (i << bitmask::promotion_offset) & bitmask::promotion;
                out.emplace_back(move);
            }
            left_capture_promotion &= left_capture_promotion - 1;
        }
    } else {
        uint64_t pawns = friendly_pieces[board::PAWN];
        // Move forward = >> or << by 8 then &= with ~occupancy_bb
        //      Also can &= with the final rank for promotion

        // white
        uint64_t pawn_single_step{ (pawns >> 8) & ~occupancy_bb & ~board::RANKS::ONE}; // March forward except promotion rank
        uint64_t pawn_double_step{ ((pawn_single_step  & board::RANKS::SIX) >> 8) & ~occupancy_bb}; // Quickly get the value before we reduce single_step
        while (pawn_single_step) {
            uint32_t to = std::countr_zero(pawn_single_step);
            uint32_t move{to + 8};
            move |= (to << bitmask::to_offset) & bitmask::to;
            out.emplace_back(move);
            pawn_single_step &= pawn_single_step - 1;
        }
        while (pawn_double_step) {
            uint32_t to = std::countr_zero(pawn_double_step);
            uint32_t move{to + 16};
            move |= (to << bitmask::to_offset) & bitmask::to;
            out.emplace_back(move);
            pawn_double_step &= pawn_double_step - 1;
        }

        uint64_t pawn_promotion{ (pawns >> 8) & ~occupancy_bb & board::RANKS::ONE}; // only Promotion
        while (pawn_promotion) {
            uint32_t to = std::countr_zero(pawn_promotion);
            for (int i=1; i<5; i++) { // Iterate for KNIGHT / BISHOP / ROOK / QUEEN Promotions
                uint32_t move{to + 8};
                move |= (to << bitmask::to_offset) & bitmask::to;
                move |= (i << bitmask::promotion_offset) & bitmask::promotion;
                out.emplace_back(move);
            }
            pawn_promotion &= pawn_promotion - 1;
        }

        // Captures = >> or << by 7 and 9 and then &= with enemy_occupancy_bb
        // Check for en passant captures

        uint64_t right_captures{ ((pawns & ~board::FILES::A) >> 9) & enemy_occupancy_bb & ~board::RANKS::ONE};

        while (right_captures) {
            uint32_t to = std::countr_zero(right_captures);
            uint32_t move{to + 9};
            move |= (to << bitmask::to_offset) & bitmask::to;
            move |= capture_flag;
            out.emplace_back(move);
            right_captures &= right_captures - 1;
        }
        uint64_t left_captures{ ((pawns & ~board::FILES::H) >> 7) & enemy_occupancy_bb & ~board::RANKS::EIGHT};
        while (left_captures) {
            uint32_t to = std::countr_zero(left_captures);
            uint32_t move{to + 7};
            move |= (to << bitmask::to_offset) & bitmask::to;
            move |= capture_flag;
            out.emplace_back(move);
            left_captures &= left_captures - 1;
        }

        const uint64_t en_passant_square = (b.m_enpassant >= 0) ? (1ULL << b.m_enpassant) : 0ULL;
        uint64_t right_enpassant_captures{ ((pawns & ~board::FILES::A) >> 9) & en_passant_square };
        while (right_enpassant_captures) {
            uint32_t to = std::countr_zero(right_enpassant_captures);
            uint32_t move{to + 9};
            move |= (to << bitmask::to_offset) & bitmask::to;
            move |= capture_flag;
            move |= (0b1 << bitmask::enpassant_offset) & bitmask::enpassant;
            out.emplace_back(move);
            right_enpassant_captures &= right_enpassant_captures - 1;
        }

        uint64_t left_enpassant_captures{ ((pawns & ~board::FILES::H) >> 7) & en_passant_square };
        while (left_enpassant_captures) {
            uint32_t to = std::countr_zero(left_enpassant_captures);
            uint32_t move{to + 7};
            move |= (to << bitmask::to_offset) & bitmask::to;
            move |= capture_flag;
            move |= (0b1 << bitmask::enpassant_offset) & bitmask::enpassant;
            out.emplace_back(move);
            left_enpassant_captures &= left_enpassant_captures - 1;
        }

        uint64_t right_capture_promotion{ ((pawns & ~board::FILES::H) >> 9) & enemy_occupancy_bb & board::RANKS::ONE};
        while (right_capture_promotion) {
            uint32_t to = std::countr_zero(right_capture_promotion);
            for (int i=1; i<5; i++) {
                // Iterate for KNIGHT / BISHOP / ROOK / QUEEN Promotions
                uint32_t move{to + 9};
                move |= (to << bitmask::to_offset) & bitmask::to;
                move |= capture_flag;
                move |= (i << bitmask::promotion_offset) & bitmask::promotion;
                out.emplace_back(move);
            }
            right_capture_promotion &= right_capture_promotion - 1;
        }

        uint64_t left_capture_promotion{ ((pawns & ~board::FILES::A) >> 7) & enemy_occupancy_bb & board::RANKS::ONE};
        while (left_capture_promotion) {
            uint32_t to = std::countr_zero(left_capture_promotion);
            for (int i=1; i<5; i++) {
                // Iterate for KNIGHT / BISHOP / ROOK / QUEEN Promotions
                uint32_t move{to + 7};
                move |= (to << bitmask::to_offset) & bitmask::to;
                move |= capture_flag;
                move |= (i << bitmask::promotion_offset) & bitmask::promotion;
                out.emplace_back(move);
            }
            left_capture_promotion &= left_capture_promotion - 1;
        }
    }







#ifdef CPPCHESSENGINE_DEBUG
        std::cout << __FILE__ << ":" << __LINE__ << " (" << __func__ << ") "  << "\n";
        std::cout << "Pawns: " << std::endl;
        b.print_bitboard(pawns);
        std::cout << "Pawn single step" << std::endl;
        b.print_bitboard(pawn_single_step);
        std::cout << "Pawn promotion" << std::endl;
        b.print_bitboard(pawn_promotion);
        std::cout << "Pawn double step" << std::endl;
        b.print_bitboard(pawn_double_step);
        std::cout << "Left captures" << std::endl;
        b.print_bitboard(left_captures);
        std::cout << "Right captures" << std::endl;
        b.print_bitboard(right_captures);
#endif





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
        uint64_t bishop_moves = attacks.Bishop(occupancy_bb, next_bishop);              // Generates the psuedo legal moves (including potential friendly captures)
        bishop_moves = bishop_moves & ~friendly_occupancy_bb;                           // Removes the friendly captures
#ifdef CPPCHESSENGINE_DEBUG
        std::cout << __FILE__ << ":" << __LINE__ << " (" << __func__ << ") "  << "\n";
        std::cout << "Bishops moves: " << std::endl;
        b.print_bitboard(bishop_moves);
#endif
        const uint32_t from = (next_bishop & bitmask::from);                            // Allocate the current square bitmasked to the first 6 bits to encode in move

        while (bishop_moves) {
            uint32_t move{from};
            const uint32_t to = std::countr_zero(bishop_moves);                         // Get the to square
            move |= (to << bitmask::to_offset) & bitmask::to;                           // Bitmask the to square to the second 6 bits to encode in move

            if ((enemy_occupancy_bb >> to) & 0b1)                                       // Detect if the move is a capture
                move |= capture_flag;
            //out.emplace_back(move);
            bishop_moves &= bishop_moves - 1;
        }

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

        const uint32_t from = (next_rook & bitmask::from);

        while (rook_moves) {
            uint32_t move{from};
            const uint32_t to = std::countr_zero(rook_moves);
            move |= (to << bitmask::to_offset) & bitmask::to;

            if ((enemy_occupancy_bb >> to) & 0b1)
                move |= capture_flag;

            //out.emplace_back(move);
            rook_moves &= rook_moves - 1;
        }
        rooks &= rooks - 1;
    }

    uint64_t queens = friendly_pieces[board::QUEEN];
    while (queens) {
        int next_queen = std::countr_zero(queens);
        uint64_t queen_moves = attacks.Queen(occupancy_bb, next_queen);
        queen_moves = queen_moves & ~friendly_occupancy_bb;
#ifdef CPPCHESSENGINE_DEBUG
        std::cout << __FILE__ << ":" << __LINE__ << " (" << __func__ << ") "  << "\n";
        std::cout << "Queen moves: " << std::endl;
        b.print_bitboard(queen_moves);
#endif

        const uint32_t from = (next_queen & bitmask::from);

        while (queen_moves) {
            uint32_t move{from};
            const uint32_t to = std::countr_zero(queen_moves);
            move |= (to << bitmask::to_offset) & bitmask::to;

            if ((enemy_occupancy_bb >> to) & 0b1)
                move |= capture_flag;

            //out.emplace_back(move);
            queen_moves &= queen_moves - 1;
        }

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
