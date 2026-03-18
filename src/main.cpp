#include "board.h"
#include "movegen.h"
#include <iostream>
#include <chrono>

int main() {
    init(); 
    Board board; 
    board.resetBoard(); 

    std::cout << "--- Starting Position ---\n";
    board.printBoard(); 

    int searchDepth = 5;

    std::cout << "\nStarting Perft Test to Depth " << searchDepth << "...\n";

    auto startTime = std::chrono::high_resolution_clock::now();

    std::uint64_t totalNodes = perft(board, searchDepth);

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = endTime - startTime;

    std::cout << "Total Nodes: " << totalNodes << "\n";
    std::cout << "Time taken: " << elapsed.count() << " seconds\n";
    std::cout << "Nodes per second: " << (uint64_t)(totalNodes / elapsed.count()) << "\n";

    return 0;
}