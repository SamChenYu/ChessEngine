#include "move_gen.h"
#define CPPCHESSENGINE_DEBUG
#include "../../board.hpp"

void move_gen::generate_legal_moves(const board& b, std::vector<int>& out) {
    const uint64_t occupancy_bb{b.get_occupancy_board()};
    // uint64_t rook_moves{attacks.Rook(board, )};
    uint64_t rook_moves{attacks.Rook(0,0)};
    

}

bool move_gen::is_king_checked(const board &board) {
    return true;
}
