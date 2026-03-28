#include "board.h"
#include "movegen.h"
#include <iostream>

int main() {
    init(); 
    Board board; 
    board.resetBoard(); 

    std::cout << "--- Starting Position ---\n";
    board.printBoard(); 

    return 0;
}