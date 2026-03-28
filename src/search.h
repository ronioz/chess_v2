#ifndef SEARCH_H
#define SEARCH_H

#include "board.h"
#include "movegen.h"

const int INF = 50000;
const int MATE_VAL = 30000;

int alphaBeta(Board& board, int depth, int alpha, int beta);
Move getBestMove(Board& board, int depth);

#endif