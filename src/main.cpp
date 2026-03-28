#include "board.h"
#include "movegen.h"
#include "search.h"
#include <iostream>
#include <string>

// Helper to convert terminal input (e2e4) to a Move object
Move parseMove(std::string input, Board& board) {
    if (input.length() != 4) return {-1, -1};
    int start = (input[0] - 'a') + (input[1] - '1') * 8;
    int end = (input[2] - 'a') + (input[3] - '1') * 8;
    
    std::vector<Move> legalMoves = generateLegalMoves(board);
    for (auto& m : legalMoves) {
        if (m.start == start && m.end == end) return m;
    }
    return {-1, -1};
}

int main() {
    init(); 
    Board board; 
    board.resetBoard(); 

    std::string input;
    int depth = 5; // You can adjust the engine's "intelligence" here

    while (true) {
        board.printBoard();
        std::cout << "\nYour move (e.g., e2e4): ";
        std::cin >> input;

        if (input == "quit") break;

        Move userMove = parseMove(input, board);
        if (userMove.start == -1) {
            std::cout << "Invalid move! Try again.\n";
            continue;
        }

        board.makeMove(userMove);
        
        // Check if the game ended after user move
        if (generateLegalMoves(board).empty()) {
            std::cout << "Game Over!\n";
            break;
        }

        std::cout << "Engine is thinking...\n";
        Move engineMove = getBestMove(board, depth);
        
        // Convert engine move to algebraic notation for the user
        char f1 = 'a' + (engineMove.start % 8), r1 = '1' + (engineMove.start / 8);
        char f2 = 'a' + (engineMove.end % 8),   r2 = '1' + (engineMove.end / 8);
        
        std::cout << "Engine plays: " << f1 << r1 << f2 << r2 << "\n";
        board.makeMove(engineMove);

        if (generateLegalMoves(board).empty()) {
            std::cout << "Game Over!\n";
            break;
        }
    }

    return 0;
}