#include "movegen.h"
#include "board.h"
#include <vector>
#define pb push_back

std::uint64_t knightAttacks[64];
std::uint64_t kingAttacks[64];
std::uint64_t pawnAttacks[2][64];

void init() {
    for(int rank = 0; rank <= 7; ++rank) {
        for(int file = 0; file <= 7; ++file){
            int square = 8 * rank + file;
            knightAttacks[square] = maskKnightAttacks(square);
            kingAttacks[square] = maskKingAttacks(square); 
            pawnAttacks[0][square] = maskPawnAttacks(0, square);
            pawnAttacks[1][square] = maskPawnAttacks(1, square);
        }
    }
}

inline int popFirstOne(std::uint64_t& bitboard) {
    int square = __builtin_ctzll(bitboard);
    bitboard &= bitboard - 1;
    return square;
}

std::uint64_t maskKnightAttacks(int square) {
    std::uint64_t bitboard = 0ULL;
    bitboard |= (1ULL << square);
    std::uint64_t attacks = 0ULL;

    attacks |= (bitboard << 17) & notAFile;
    attacks |= (bitboard << 10) & notABFile; 
    attacks |= (bitboard >> 15) & notAFile;  
    attacks |= (bitboard >> 6)  & notABFile; 

    attacks |= (bitboard << 15) & notHFile;
    attacks |= (bitboard << 6)  & notGHFile; 
    attacks |= (bitboard >> 17) & notHFile;  
    attacks |= (bitboard >> 10) & notGHFile; 

    return attacks;
}

std::uint64_t maskKingAttacks(int square) {
    std::uint64_t bitboard = 0ULL;
    bitboard |= (1ULL << square);
    std::uint64_t attacks = 0ULL;

    attacks |= (bitboard << 8);            
    attacks |= (bitboard >> 8);            
    attacks |= (bitboard << 1) & notAFile;   
    attacks |= (bitboard << 9) & notAFile;
    attacks |= (bitboard >> 7) & notAFile;    

    attacks |= (bitboard >> 1) & notHFile;      
    attacks |= (bitboard << 7) & notHFile;      
    attacks |= (bitboard >> 9) & notHFile;    

    return attacks;
}

std::uint64_t maskPawnAttacks(int side, int square) {
    std::uint64_t bitboard = 0ULL;
    bitboard |= (1ULL << square);
    std::uint64_t attacks = 0ULL;

    if(side) {
        attacks |= (bitboard >> 9) & notHFile;
        attacks |= (bitboard >> 7) & notAFile;
    } else {
        attacks |= (bitboard << 9) & notAFile;
        attacks |= (bitboard << 7) & notHFile;
    }

    return attacks;
}

std::uint64_t maskBishopAttacks(int square) {
    std::uint64_t attacks = 0ULL;
    int rank = square / 8;
    int file = square % 8;

    int r, f;

    for(r = rank+1, f = file+1; r <= 6 && f <= 6; ++r, ++f)
        attacks |= (1ULL << (r*8+f));
    
    for(r = rank-1, f = file+1; r >= 1 && f <= 6; --r, ++f)
        attacks |= (1ULL << (r*8+f));

    for(r = rank+1, f = file-1; r <= 6 && f >= 1; ++r, --f)
        attacks |= (1ULL << (r*8+f));

    for(r = rank-1, f = file-1; r >= 1 && f >= 1; --r, --f)
        attacks |= (1ULL << (r*8+f));

    return attacks;
}

std::uint64_t maskRookAttacks(int square) {
    std::uint64_t attacks = 0ULL;
    int rank = square / 8;
    int file = square % 8;

    for(int r = rank+1; r <= 6; ++r)
        attacks |= (1ULL << (r*8 + file));

    for(int r = rank-1; r >= 1; --r)
        attacks |= (1ULL << (r*8 + file));

    for(int f = file+1; f <= 6; ++f)
        attacks |= (1ULL << (rank*8 + f));

    for(int f = file-1; f >= 1; --f)
        attacks |= (1ULL << (rank*8 + f));

    return attacks;
}

std::uint64_t getRookAttacks(int square, std::uint64_t occupancy) {
    std::uint64_t attacks = 0ULL;
    int rank = square / 8;
    int file = square % 8;

    for(int r = rank + 1; r <= 7; ++r) {
        attacks |= (1ULL << (r * 8 + file));
        if (occupancy & (1ULL << (r * 8 + file))) break;
    }
    
    for(int r = rank - 1; r >= 0; --r) {
        attacks |= (1ULL << (r * 8 + file));
        if (occupancy & (1ULL << (r * 8 + file))) break;
    }
    
    for(int f = file + 1; f <= 7; ++f) {
        attacks |= (1ULL << (rank * 8 + f));
        if (occupancy & (1ULL << (rank * 8 + f))) break;
    }
    
    for(int f = file - 1; f >= 0; --f) {
        attacks |= (1ULL << (rank * 8 + f));
        if (occupancy & (1ULL << (rank * 8 + f))) break;
    }

    return attacks;
}

std::uint64_t getBishopAttacks(int square, std::uint64_t occupancy) {
    std::uint64_t attacks = 0ULL;
    int rank = square / 8;
    int file = square % 8;
    int r, f;

    for(r = rank+1, f = file+1; r <= 7 && f <= 7; ++r, ++f){
        attacks |= (1ULL << (r*8+f));
        if(occupancy & (1ULL << (r*8+f))) break;
    }
    
    for(r = rank-1, f = file+1; r >= 0 && f <= 7; --r, ++f){
        attacks |= (1ULL << (r*8+f));
        if(occupancy & (1ULL << (r*8+f))) break;
    }

    for(r = rank+1, f = file-1; r <= 7 && f >= 0; ++r, --f){
        attacks |= (1ULL << (r*8+f));
        if(occupancy & (1ULL << (r*8+f))) break;
    }

    for(r = rank-1, f = file-1; r >= 0 && f >= 0; --r, --f){
        attacks |= (1ULL << (r*8+f));
        if(occupancy & (1ULL << (r*8+f))) break;
    }

    return attacks;
}

std::vector<Move> generateMoves(Board& board) {
    std::vector<Move> moves;
    moves.reserve(256);

    int color = board.sideToMove;
    std::uint64_t friendlyPieces = (color == WHITE) ? board.whitePieces : board.blackPieces;
    std::uint64_t enemyPieces = (color == WHITE) ? board.blackPieces : board.whitePieces;

    //KNIGHTS

    std::uint64_t knights = board.bitboards[color][KNIGHT];

    while(knights) {
        int startSquare = popFirstOne(knights);

        std::uint64_t attacks = knightAttacks[startSquare] & ~friendlyPieces;

        while(attacks) {
            int toSquare = popFirstOne(attacks);
            moves.pb({startSquare, toSquare});
        }
    }

    //KING

    std::uint64_t king = board.bitboards[color][KING];

    while(king) {
        int startSquare = popFirstOne(king);

        std::uint64_t attacks = kingAttacks[startSquare] & ~friendlyPieces;

        while(attacks) {
            int toSquare = popFirstOne(attacks);
            moves.pb({startSquare, toSquare});
        }
    }

    //CASTLING

    //WHITE (BOTH SIDES)

    if(color == WHITE) {
        if(board.castlingRights & 1) {
            if(!(board.allPieces & ((1ULL << 5) | (1ULL << 6)))) {
                if(!board.isSquareAttacked(4, BLACK) && !board.isSquareAttacked(5, BLACK)){
                    moves.pb({4, 6});
                }
            }
        }

        if(board.castlingRights & 2) {
            if(!(board.allPieces & ((1ULL << 2) | (1ULL << 3)))) {
                if(!board.isSquareAttacked(1, BLACK) && !board.isSquareAttacked(2, BLACK) && !board.isSquareAttacked(3, BLACK)){
                    moves.pb({4, 2});
                }
            }
        }
    }

    else {
        if(board.castlingRights & 4) {
            if(!(board.allPieces & ((1ULL << 61) | (1ULL << 62)))) {
                if(!board.isSquareAttacked(60, WHITE) && !board.isSquareAttacked(61, WHITE)){
                    moves.pb({60, 62});
                }
            }
        }

        if(board.castlingRights & 8) {
            if(!(board.allPieces & ((1ULL << 57) | (1ULL << 58) | (1ULL << 59)))) {
                if(!board.isSquareAttacked(60, WHITE) && !board.isSquareAttacked(59, WHITE)){
                    moves.pb({60, 58});
                }
            }
        }
    }

    //ROOKS

    std::uint64_t rooks = board.bitboards[color][ROOK];

    while (rooks) {
        int startSquare = popFirstOne(rooks);

        std::uint64_t attacks = getRookAttacks(startSquare, board.allPieces) & ~friendlyPieces;

        while (attacks) {
            int toSquare = popFirstOne(attacks);
            moves.pb({startSquare, toSquare});
        }
    }

    //BISHOPS

    std::uint64_t bishops = board.bitboards[color][BISHOP];

    while (bishops) {
        int startSquare = popFirstOne(bishops);

        std::uint64_t attacks = getBishopAttacks(startSquare, board.allPieces) & ~friendlyPieces;

        while (attacks) {
            int toSquare = popFirstOne(attacks);
            moves.pb({startSquare, toSquare});
        }
    }

    //QUEENS

    std::uint64_t queens = board.bitboards[color][QUEEN];

    while(queens) {
        int startSquare = popFirstOne(queens);

        std::uint64_t attacks = (getRookAttacks(startSquare, board.allPieces) | getBishopAttacks(startSquare, board.allPieces)) & ~friendlyPieces;
        
        while (attacks) {
            int toSquare = popFirstOne(attacks);
            moves.pb({startSquare, toSquare});
        }
    } 

    //PAWNS

    std::uint64_t pawns = board.bitboards[color][PAWN];
    std::uint64_t emptySquares = ~board.allPieces;

    if(color == WHITE) {
        std::uint64_t singlePushes = (pawns << 8) & emptySquares;
        std::uint64_t temp = singlePushes;
        
        while(temp) {
            int toSquare = popFirstOne(temp);
            moves.pb({toSquare - 8, toSquare});
        } //single push

        const std::uint64_t rank4mask = 0x00000000FF000000ULL;
        std::uint64_t doublePushes = (singlePushes << 8) & emptySquares & rank4mask;

        while(doublePushes) {
            int toSquare = popFirstOne(doublePushes);
            moves.pb({toSquare-16, toSquare});
        }

        temp = pawns;
        while (temp) {
            int startSquare = popFirstOne(temp);
            
            std::uint64_t attacks = pawnAttacks[WHITE][startSquare] & enemyPieces;
            
            while (attacks) {
                int toSquare = popFirstOne(attacks);
                moves.pb({startSquare, toSquare});
            }
        } //captures

        if(board.enPassantSquare != -1) {
            std::uint64_t epMask = (1ULL << board.enPassantSquare);
            std::uint64_t tempEp = pawns;

            while(tempEp) {
                int startSquare = popFirstOne(tempEp);
                if(pawnAttacks[WHITE][startSquare] & epMask) {
                    moves.pb({startSquare, board.enPassantSquare});
                }
            }
        }
    } else {
        std::uint64_t singlePushes = (pawns >> 8) & emptySquares;
        std::uint64_t temp = singlePushes;
        
        while(temp) {
            int toSquare = popFirstOne(temp);
            moves.pb({toSquare + 8, toSquare});
        } //single push

        const std::uint64_t rank5mask = 0x000000FF00000000ULL;;
        std::uint64_t doublePushes = (singlePushes >> 8) & emptySquares & rank5mask;

        while(doublePushes) {
            int toSquare = popFirstOne(doublePushes);
            moves.pb({toSquare + 16, toSquare});
        }

        temp = pawns;
        while (temp) {
            int startSquare = popFirstOne(temp);
            
            std::uint64_t attacks = pawnAttacks[BLACK][startSquare] & enemyPieces;
            
            while (attacks) {
                int toSquare = popFirstOne(attacks);
                moves.pb({startSquare, toSquare});
            }
        } //captures

        if(board.enPassantSquare != -1) {
            std::uint64_t epMask = (1ULL << board.enPassantSquare);
            std::uint64_t tempEp = pawns;

            while(tempEp) {
                int startSquare = popFirstOne(tempEp);
                if (pawnAttacks[BLACK][startSquare] & epMask) {
                    moves.pb({startSquare, board.enPassantSquare});
                }
            }
        }
    }

    return moves;
}

std::vector<Move> generateLegalMoves(Board& board) {
    std::vector<Move> allMoves = generateMoves(board);
    std::vector<Move> legalMoves;
    legalMoves.reserve(256);

    int color = board.sideToMove;

    for(const Move& move: allMoves) {
        board.makeMove(move);

        int kingSquare = __builtin_ctzll(board.bitboards[color][KING]);

        if(!board.isSquareAttacked(kingSquare, color^1)) {
            legalMoves.pb(move);
        }

        board.unmakeMove();
    }

    return legalMoves;
}

std::uint64_t perft(Board& board, int depth) {
    if(depth == 0) {
        return 1ULL;
    }

    std::uint64_t nodes = 0ULL;
    std::vector<Move> moves = generateLegalMoves(board);

    for(const Move& move: moves) {
        board.makeMove(move);
        nodes += perft(board, depth-1);
        board.unmakeMove();
    }

    return nodes;
}