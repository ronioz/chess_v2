#ifndef MOVEGEN_H
#define MOVEGEN_H

#include <cstdint>
#include <vector>
#include "board.h"

struct Move {
    int start;
    int end;
};

inline int popFirstOne(std::uint64_t& bitboard);

constexpr std::uint64_t notAFile  = 0xFEFEFEFEFEFEFEFEULL; 
constexpr std::uint64_t notABFile = 0xFCFCFCFCFCFCFCFCULL;
constexpr std::uint64_t notHFile  = 0x7F7F7F7F7F7F7F7FULL; 
constexpr std::uint64_t notGHFile = 0x3F3F3F3F3F3F3F3FULL; 

extern std::uint64_t knightAttacks[64];
extern std::uint64_t kingAttacks[64];
extern std::uint64_t pawnAttacks[2][64];

std::uint64_t maskKnightAttacks(int square);
std::uint64_t maskKingAttacks(int square);
std::uint64_t maskPawnAttacks(int side, int square);
std::uint64_t maskBishopAttacks(int square);
std::uint64_t maskRookAttacks(int square);
std::uint64_t getRookAttacks(int square, std::uint64_t occupancy);
std::uint64_t getBishopAttacks(int square, std::uint64_t occupancy);
std::vector<Move> generateMoves(const Board& board);
void init();

#endif