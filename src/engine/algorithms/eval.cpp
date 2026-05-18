#include "eval.hpp"

#include <cmath>
#include "pst.hpp"

static_assert(pst::PAWN[pst::mirror(0)] == pst::PAWN[56]);

float eval::get_eval(const board& b) {


    int white_material{0}, black_material{0};
    int white_pos{0}, black_pos{0};

    for (int i=0; i<6; i++) {
        uint64_t w_board = b.m_white[i];
        uint64_t b_board = b.m_black[i];

        white_material += std::popcount(w_board) * pst::PIECE_VALUES[i];
        black_material += std::popcount(b_board) * pst::PIECE_VALUES[i];

        while (w_board) {
            const int next_sq = std::countr_zero(w_board);
            white_pos += pst::PST[i][pst::mirror(next_sq)];
            w_board &= w_board - 1;
        }

        while (b_board) {
            const int next_sq = std::countr_zero(b_board);
            black_pos += pst::PST[i][next_sq];
            b_board &= b_board - 1;
        }
    }

    // greater material difference means position matters less
    const int material_diff = std::abs(white_material - black_material);

    float material_weight;
    if (material_diff > 8)
        material_weight = 0.9f;
    else if (material_diff > 6)
        material_weight = 0.7f;
    else if (material_diff > 4)
        material_weight = 0.5f;
    else if (material_diff > 1)
        material_weight = 0.3;
    else
        material_weight = 0.1;

    float position_weight = 1.0f - material_weight;

    const int material_diff_signed = white_material - black_material;
    float material_value = static_cast<float>(material_diff_signed) * material_weight;
    material_value *= material_weight;

    constexpr int position_scalar = 1169; // max points given full traditional board

    const float white_pos_value =
        (static_cast<float>(white_pos) / position_scalar) * position_weight;

    const float black_pos_value =
        (static_cast<float>(black_pos) / position_scalar) * position_weight;

    const float eval = white_pos_value - black_pos_value + material_value;
    return eval;
}