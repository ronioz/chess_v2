#include "search.h"

int alphaBeta(Board& board, int depth, int alpha, int beta) {
    if(depth == 0) return board.evaluate();

    std::vector<Move> moves = generateLegalMoves(board);
    if(moves.empty()) {
        if(board.isSquareAttacked(__builtin_ctzll(board.bitboards[board.sideToMove][KING]), board.sideToMove ^ 1)) {
            return -MATE_VAL - depth;
        }
        return 0; // STALEMATE
    }

    for(const Move& move : moves) {
        board.makeMove(move);
        int score = -alphaBeta(board, depth-1, -beta, -alpha);
        board.unmakeMove();

        if(score >= beta){
            return beta;
        }

        if(alpha < score){
            alpha = score;
        }
    }

    return alpha;
}

Move getBestMove(Board& board, int depth) {
    std::vector<Move> moves = generateLegalMoves(board);
    Move bestMove = moves[0];
    int bestScore = -INF;

    for(const Move& move : moves) {
        board.makeMove(move);
        int score = -alphaBeta(board, depth-1, -INF, INF);
        board.unmakeMove();

        if(score > bestScore){
            bestScore = score;
            bestMove = move;
        }
    }

    return bestMove;
}