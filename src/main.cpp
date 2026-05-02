#include <iostream>
#include <string>

int main() {
    // Let's just assume that we are gonna just std::cin the FEN string

    std::cout << "Enter FEN String: " ;
    std::string input;
    std::getline(std::cin, input);

    std::cout << input << std::endl;

    return 0;
}