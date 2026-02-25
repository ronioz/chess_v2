#include "board.h"
#include "movegen.h"
#include <vector>
#include <iostream>

void printMoves(std::vector<Move> moves) {
    printf("Total moves generated: %d\n", moves.size());
    for(const Move& move: moves) {
        char startFile = 'a' + (move.start % 8);
        char startRank = '1' + (move.start / 8);

        char endFile = 'a' + (move.end % 8);
        char endRank = '1' + (move.end / 8);

        std::cout << startFile << startRank << endFile << endRank << std::endl;
    }

    printf("\n");
}

int main() {
    init();
    Board board;
    board.resetBoard();

    std::vector<Move> moves = generateMoves(board);
    printMoves(moves);
    
    return 0;
}