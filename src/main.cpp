
#include "board.hpp"
#include <iostream>
#include <string>


int main() {
    // Let's just assume that we are gonna just std::cin the FEN string

    std::cout << "Enter FEN String: " ;
    std::string input;
    std::getline(std::cin, input);

    try {
        board b1 = board(input);
    } catch (std::invalid_argument &e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}