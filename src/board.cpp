#include <iostream>
#include "board.h"
#include "movegen.h"
#include <vector>
#define pb push_back

Board::Board() {
    resetBoard();
}

void Board::resetBoard() {
    for(int c = WHITE; c <= BLACK; ++c){
        for(int p = PAWN; p <= KING; ++p){
            bitboards[c][p] = 0ULL;
        }
    }

    bitboards[WHITE][PAWN]   = 0x000000000000FF00ULL;
    bitboards[WHITE][KNIGHT] = 0x0000000000000042ULL;
    bitboards[WHITE][BISHOP] = 0x0000000000000024ULL;
    bitboards[WHITE][ROOK]   = 0x0000000000000081ULL;
    bitboards[WHITE][QUEEN]  = 0x0000000000000008ULL;
    bitboards[WHITE][KING]   = 0x0000000000000010ULL;

    bitboards[BLACK][PAWN]   = 0x00FF000000000000ULL;
    bitboards[BLACK][KNIGHT] = 0x4200000000000000ULL;
    bitboards[BLACK][BISHOP] = 0x2400000000000000ULL;
    bitboards[BLACK][ROOK]   = 0x8100000000000000ULL;
    bitboards[BLACK][QUEEN]  = 0x0800000000000000ULL;
    bitboards[BLACK][KING]   = 0x1000000000000000ULL;

    whitePieces = 0ULL;
    blackPieces = 0ULL;

    for(int p = PAWN; p <= KING; ++p){
        whitePieces |= bitboards[WHITE][p];
        blackPieces |= bitboards[BLACK][p];
    }

    allPieces = whitePieces | blackPieces;

    sideToMove = WHITE;
}

void Board::printBoard() {
    char pieces[2][6] = {
        {'P', 'N', 'B', 'R', 'Q', 'K'},
        {'p', 'n', 'b', 'r', 'q', 'k'}
    };

    for(int rank = 7; rank >= 0; --rank){
        for(int file = 0; file < 8; ++file){
            int sq = rank * 8 + file;
            std::uint64_t mask = (1ULL << sq);

            char piece = '.';

            for(int c = WHITE; c <= BLACK; ++c){
                for(int p = PAWN; p <= KING; ++p){
                    if(bitboards[c][p] & mask) piece = pieces[c][p];
                }
            }

            printf("%c ", piece);
        }

        printf("\n");
    }
}

void Board::makeMove(Move move) {
    int start = move.start;
    int end = move.end;
    int color = sideToMove;
    int enemyColor = color ^ 1;

    int movedPiece = -1;
    int capturedPiece = -1;

    for(int p = PAWN; p <= KING; ++p) {
        if(bitboards[color][p] & (1ULL << start)){
            movedPiece = p;
            break;
        }
    }

    bitboards[color][movedPiece] &= ~(1ULL << start); //erase
    bitboards[color][movedPiece] |= (1ULL << end);  //draw

    if(allPieces & (1ULL << end)){
        for(int p = PAWN; p <= KING; ++p) {
            if(bitboards[enemyColor][p] & (1ULL << end)){
                capturedPiece = p;
                bitboards[enemyColor][p] &= ~(1ULL << end);
                break;
            }
        }
    }

    UndoState state;
    state.move = move;
    state.movedPiece = movedPiece;
    state.capturedPiece = capturedPiece;
    history.pb(state);

    whitePieces = 0ULL;
    blackPieces = 0ULL;

    for(int p = PAWN; p <= KING; ++p) {
        whitePieces |= bitboards[WHITE][p];
        blackPieces |= bitboards[BLACK][p];
    }

    allPieces = whitePieces | blackPieces;

    sideToMove ^= 1;
}

void Board::unmakeMove() {
    if(history.empty()) return;

    UndoState undoState = history.back();
    history.pop_back();

    sideToMove ^= 1;
    int color = sideToMove;
    int enemyColor = color ^ 1;

    int start = undoState.move.start;
    int end = undoState.move.end;

    bitboards[color][undoState.movedPiece] &= ~(1ULL << end);
    bitboards[color][undoState.movedPiece] |= (1ULL << start);

    if(undoState.capturedPiece != -1) {
        bitboards[enemyColor][undoState.capturedPiece] |= (1ULL << end);
    }

    whitePieces = 0ULL;
    blackPieces = 0ULL;

    for(int p = PAWN; p <= KING; ++p) {
        whitePieces |= bitboards[WHITE][p];
        blackPieces |= bitboards[BLACK][p];
    }

    allPieces = whitePieces | blackPieces;
}