C++ Bitboard Chess Engine
A high-performance chess engine built from scratch in C++, utilizing 64-bit integer bitboards for board representation and move generation.

Current Status: Phase 1 Complete (Move Generation)
The engine currently possesses a fully functional Pseudo-Legal Move Generator. It can take a board state, analyze the position using bitwise operations, and generate a list of all physically possible moves for the active color, including directional pawn pushes, leaper jumps, and blocked sliding raycasts.

⚙️ Core Architecture & Features
1. Bitboard Board Representation (board.cpp / board.h)
64-bit State: The board is represented using an array of std::uint64_t bitboards, separating pieces by color (White/Black) and type (Pawn, Knight, Bishop, Rook, Queen, King).

Aggregate Bitboards: Maintains updated whitePieces, blackPieces, and allPieces bitboards for instant occupancy lookups.

Initialization: capable of resetting to the standard chess starting position and printing the board in an ASCII format for visual debugging.

2. Pre-calculated Leaper Attacks (movegen.cpp / movegen.h)
Knights & Kings: Attack patterns for non-sliding pieces are calculated once at startup (init()) and stored in lookup arrays (knightAttacks[64], kingAttacks[64]).

Pawn Captures: Pre-calculated directional capture tables (pawnAttacks[2][64]) handling White (Up) and Black (Down) pawn attacks.

Edge Wrap Prevention: Mathematical masking (notAFile, notHFile, etc.) safely prevents pieces from wrapping around the edges of the board.

3. On-The-Fly Raycasting for Sliders
Bishops, Rooks, & Queens: Utilizes dynamic for loops to shoot rays across the board (getRookAttacks, getBishopAttacks).

Occupancy Detection: Rays stop instantly upon intersecting with the allPieces bitboard, capturing enemy pieces while preventing phasing through blockers.

Mask Generation: Includes foundational "Outer Edge Exclusion" masks (maskRookAttacks, maskBishopAttacks) prepared for a future upgrade to Magic Bitboards.

4. Hardware-Accelerated Bit Manipulation
Trailing Zero Count: Leverages Apple Clang's __builtin_ctzll hardware intrinsic to extract piece locations from bitboards in a single CPU cycle.

Brian Kernighan's Algorithm: Uses bb &= bb - 1 to systematically clear the least significant bit, allowing loop-free piece extraction.

Bitwise Pawn Logic: Calculates single and double pawn pushes simultaneously using fast bitwise shifts (<< 8, >> 8) and rank-mask filtering.

📂 Project Structure
main.cpp: The entry point. Initializes lookup tables, sets up the board, and prints generated moves.

board.h / board.cpp: Manages the state, memory, and initialization of the 64-bit chessboard.

movegen.h / movegen.cpp: Contains the math, raycasting, and generateMoves() logic for producing standard algebraic notation moves (e.g., e2e4).

🗺️ Roadmap (Next Steps)
[ ] Make/Unmake Move: Implement functions to update the bitboards when a move is played and restore them when backtracking.

[ ] Legal Move Filtering: Filter out pseudo-legal moves that leave the King in check.

[ ] Special Moves: Implement Castling, En Passant, and Pawn Promotion.

[ ] Evaluation: Write a function to score a board position (material + positional bonuses).

[ ] Search Algorithm: Implement Minimax with Alpha-Beta Pruning to look ahead and find the best move.