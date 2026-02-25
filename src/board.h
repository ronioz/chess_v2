#ifndef BOARD_H
#define BOARD_H

#include <cstdint>

enum Color { WHITE, BLACK };
enum Pieces { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };

class Board {
    public:
        Board();
        void printBoard();
        void resetBoard();
    
        std::uint64_t bitboards[2][6];
        std::uint64_t whitePieces;
        std::uint64_t blackPieces;
        std::uint64_t allPieces;
        
        int sideToMove;
};

#endif 