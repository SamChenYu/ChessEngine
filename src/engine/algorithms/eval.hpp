#pragma once
#include <array>
#include "../../board.hpp"

class board;

struct eval {
public:
    static float get_eval(const board& b);
};