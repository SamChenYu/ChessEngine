#include "eval.hpp"

#include <cmath>
#include "pst.hpp"

float eval::get_eval(const board& b) {

    int white_material{0}, black_material{0};
    int white_pos{0}, black_pos{0};

    for (int i=0; i<6; i++) {
        unsigned long long w_board = b.m_white[i];
        unsigned long long b_board = b.m_black[i];

        white_material += std::popcount(w_board) * pst::PIECE_VALUES[i];
        black_material += std::popcount(b_board) * pst::PIECE_VALUES[i];

        while (w_board) {
            int next_sq = std::countr_zero(w_board);
            white_pos += pst::PST[i][pst::mirror(next_sq)];
            w_board &= w_board - 1;
        }

        while (b_board) {
            int next_sq = std::countr_zero(b_board);
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

    // Todo: no clue what I previously wrote here, will need to revisit this later
    int material_diff_prev = white_material - black_material;
    float material_value;
    if (material_diff_prev > 0) {
        material_value = 1.0f / (1.1f + std::exp(-(material_diff) + 4));
    } else if (material_diff_prev < 0) {
        material_diff_prev *= -1;
        material_value = 1.0f / (1.1f + std::exp(-(material_diff) + 4));
        material_diff_prev *= -1;
    } else {
        material_value = 0.0f;
    }
    material_value *= material_weight;

    constexpr int position_scalar = 1169; // max points given full traditional board

    float white_pos_value = (white_pos / position_scalar) * position_weight;
    float black_pos_value = (black_pos / position_scalar) * position_weight;
    return (white_pos_value - black_pos_value + material_value);
}