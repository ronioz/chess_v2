#include "board.h"
#include "movegen.h"
#include <vector>
#include <iostream>

void printMoves(std::vector<Move> moves) {
    printf("Total moves generated: %d\n", (int)moves.size());
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

    printf("Starting Position!\n");
    board.printBoard();

    std::vector<Move> moves = generateMoves(board);
    Move e2e4 = {-1, -1};
    for(const Move& m : moves) {
        if(m.start == 12 && m.end == 28) {
            e2e4 = m;
            break;
        }
    }

    if(e2e4.start == -1) {
        printf("Cannot play e2e4");
        return 0;
    }

    printf("Playing move e2-e4!\n");
    board.makeMove(e2e4);
    printf("Updated board:\n");
    board.printBoard();

    printf("Unmake move e2-e4!\n");
    board.unmakeMove();
    printf("Updated board:\n");
    board.printBoard();
    
    return 0;
}