#include <iostream>
#include "board.h"
#include "movegen.h"
#include <vector>
#define pb push_back

const int castlingUpdate[64] = {
    13, 15, 15, 15, 12, 15, 15, 14,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
7, 15, 15, 15,  3, 15, 15, 11
};

Board::Board() {
    resetBoard();
}

void Board::resetBoard() {
    for(int c = WHITE; c <= BLACK; ++c){
        for(int p = PAWN; p <= KING; ++p){
            bitboards[c][p] = 0ULL;
        }

        sideToMove = WHITE;
        castlingRights = 15;
        enPassantSquare = -1;
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

    if(allPieces & (1ULL << end)){
        for(int p = PAWN; p <= KING; ++p) {
            if(bitboards[enemyColor][p] & (1ULL << end)){
                capturedPiece = p;
                break; 
            }
        }
    }

    UndoState state;
    state.move = move;
    state.movedPiece = movedPiece;
    state.capturedPiece = capturedPiece;
    state.castlingRights = castlingRights;
    state.enPassantSquare = enPassantSquare;
    history.pb(state);

    bitboards[color][movedPiece] &= ~(1ULL << start);
    bitboards[color][movedPiece] |= (1ULL << end);

    if(movedPiece == KING && (end - start == 2 || start - end == 2)) {
        if(end == 6) {
            bitboards[WHITE][ROOK] &= ~(1ULL << 7);
            bitboards[WHITE][ROOK] |= (1ULL << 5);
        } else if(end == 2) {
            bitboards[WHITE][ROOK] &= ~(1ULL << 0);
            bitboards[WHITE][ROOK] |= (1ULL << 3);
        } else if(end == 62) {
            bitboards[BLACK][ROOK] &= ~(1ULL << 63);
            bitboards[BLACK][ROOK] |= (1ULL << 61);
        } else {
            bitboards[BLACK][ROOK] &= ~(1ULL << 56);
            bitboards[BLACK][ROOK] |= (1ULL << 59);
        }
    } //CASTLING

    if (movedPiece == PAWN && (end - start == 16 || start - end == 16)) {
        enPassantSquare = (color == WHITE) ? start + 8 : start - 8;
    } else {
        enPassantSquare = -1;
    }

    if(capturedPiece != -1) {
        bitboards[enemyColor][capturedPiece] &= ~(1ULL << end); 
    } else if (movedPiece == PAWN && end == state.enPassantSquare) {
        int victimSquare = (color == WHITE) ? end - 8 : end + 8;
        bitboards[enemyColor][PAWN] &= ~(1ULL << victimSquare);
    }

    castlingRights &= castlingUpdate[start];
    castlingRights &= castlingUpdate[end];

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

    castlingRights = undoState.castlingRights;
    enPassantSquare = undoState.enPassantSquare;

    sideToMove ^= 1;
    int color = sideToMove;
    int enemyColor = color ^ 1;

    int start = undoState.move.start;
    int end = undoState.move.end;

    bitboards[color][undoState.movedPiece] &= ~(1ULL << end);
    bitboards[color][undoState.movedPiece] |= (1ULL << start);

    if(undoState.movedPiece == KING && (start - end == 2 || end - start == 2)) {
        if(end == 6) {
            bitboards[WHITE][ROOK] &= ~(1ULL << 5);
            bitboards[WHITE][ROOK] |= (1ULL << 7);
        } else if(end == 2) {
            bitboards[WHITE][ROOK] &= ~(1ULL << 3);
            bitboards[WHITE][ROOK] |= (1ULL << 0);
        } else if(end == 62) {
            bitboards[BLACK][ROOK] &= ~(1ULL << 61);
            bitboards[BLACK][ROOK] |= (1ULL << 63);
        } else {
            bitboards[BLACK][ROOK] &= ~(1ULL << 59);
            bitboards[BLACK][ROOK] |= (1ULL << 56);
        }
    } //UNCASTLING

    if(undoState.capturedPiece != -1) {
        bitboards[enemyColor][undoState.capturedPiece] |= (1ULL << end);
    } else if (undoState.movedPiece == PAWN && end == undoState.enPassantSquare) {
        int victimSquare = (color == WHITE) ? end - 8 : end + 8;
        bitboards[enemyColor][PAWN] |= (1ULL << victimSquare);
    }

    whitePieces = 0ULL;
    blackPieces = 0ULL;

    for(int p = PAWN; p <= KING; ++p) {
        whitePieces |= bitboards[WHITE][p];
        blackPieces |= bitboards[BLACK][p];
    }

    allPieces = whitePieces | blackPieces;
}

bool Board::isSquareAttacked(int square, int attackingColor) {
    int defendingColor = attackingColor ^ 1;
    std::uint64_t diagonalAttackers = bitboards[attackingColor][BISHOP] | bitboards[attackingColor][QUEEN];
    std::uint64_t lineAttackers = bitboards[attackingColor][ROOK] | bitboards[attackingColor][QUEEN];

    if(knightAttacks[square] & bitboards[attackingColor][KNIGHT]) return true;
    if(kingAttacks[square] & bitboards[attackingColor][KING]) return true;
    if(pawnAttacks[defendingColor][square] & bitboards[attackingColor][PAWN]) return true;
    if(getBishopAttacks(square, allPieces) & diagonalAttackers) return true;
    if(getRookAttacks(square, allPieces) & lineAttackers) return true;

    return false;
}

int Board::evaluate() {
    int score = 0;
    int values[] = {100, 300, 300, 500, 900, 20000};

    for(int p = PAWN; p <= KING; ++p) {
        std::uint64_t w_pieces = bitboards[WHITE][p];
        while(w_pieces) {
            popFirstOne(w_pieces);
            score += values[p];
        }
    }

    for(int p = PAWN; p <= KING; ++p) {
        std::uint64_t w_pieces = bitboards[BLACK][p];
        while(w_pieces) {
            popFirstOne(w_pieces);
            score -= values[p];
        }
    }

    return (sideToMove == WHITE) ? score : -score;
}

bool Board::isCapture(Move move) {
    return allPieces & (1ULL << move.end);
}