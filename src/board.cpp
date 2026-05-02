#include "board.h"

board::board() : white_turn{true}, castling{15},
            enpassant{0}, halfmove_clock{0}, fullmove_clock{0} {
    // init the boards themselves
}