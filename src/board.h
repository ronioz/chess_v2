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
    int castlingRights;
    int enPassantSquare;
};

class Board {
    public:
        Board();
        void printBoard();
        void resetBoard();
        void makeMove(Move move);
        void unmakeMove();
        bool isSquareAttacked(int square, int attackingColor);
        int evaluate();
        bool isCapture(Move move);

        std::uint64_t bitboards[2][6];
        std::uint64_t whitePieces;
        std::uint64_t blackPieces;
        std::uint64_t allPieces;
        
        int sideToMove;
        int castlingRights;
        int enPassantSquare;

        std::vector<UndoState> history;
};

#endif 