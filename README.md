# C++ Chess Engine

- [x] Minimax with alpha beta pruning
- [ ] Validate legal moves within the search algorithm
- [ ] Complete board::make_move
- [ ] Implement board::undo_move (needs to update the uint32_t moves)
- [ ] Utilize undo_move instead of copy semantics and causing allocations

Roadmap
- [ ] Implement a better evaluation function
- [ ] Implement transposition table / zobrist hashing
- [ ] Fix horrible debug defines

Current benchmark on `-O3` from starting position depth 9 single threaded averaging ~3.9 million nodes per second (M1 Max).

## Quick Lookup


``` c++
    // rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR
    // a8....h8...........................a1....h1


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


    Moves are encoded into 32 bit integers
    bits 0 – 5   FROM
    bits 6 – 11  TO
    bits 12     CAPTURE FLAG
    bits 13     EN PASSANT FLAG
    bits 14 - 15     CASTLE FLAG
        00 = No Castling
        01 = Kingside
        10 = Queenside
    bits 16 – 19 PROMOTION PIECE
        000 = No Promotion
        001 = Knight
        010 = Bishop
        011 = Rook
        100 = QUEEN

    // General search is going to be made by copying board states,
    // not by moving / unmoving so we will disregard CAPTURE, and
    // DOUBLE PAWN PUSH


    enum bitmask : uint32_t {
        from        =       0b00000000000000000000000000111111, from_offset = 0,
        to          =       0b00000000000000000000111111000000, to_offset = 6,
        capture     =       0b00000000000000000001000000000000, capture_offset = 12,
        enpassant   =       0b00000000000000000010000000000000, enpassant_offset = 13,
        castling    =       0b00000000000000001100000000000000, castling_offset = 14,
        promotion   =       0b00000000000011110000000000000000, promotion_offset = 16
    };
```
