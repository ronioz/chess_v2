#ifndef BOARD_H
#define BOARD_H

#include <cstdint>
#include <vector>

enum Color { WHITE, BLACK };
enum Pieces { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };

struct Move {
    int start;
    int end;
};

struct UndoState {
    Move move;
    int movedPiece;
    int capturedPiece;
};

class Board {
    public:
        Board();
        void printBoard();
        void resetBoard();
        void makeMove(Move move);
        void unmakeMove();

        std::uint64_t bitboards[2][6];
        std::uint64_t whitePieces;
        std::uint64_t blackPieces;
        std::uint64_t allPieces;
        
        int sideToMove;

        std::vector<UndoState> history;
};

#endif 