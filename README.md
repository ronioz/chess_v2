# C++ Bitboard Chess Engine

A high-performance chess engine built from scratch in C++, utilizing 64-bit integer bitboards for board representation and move generation. The project architecture is designed to support a high-speed C++ engine core that will eventually interface with a custom Python-based UI.

## Current Status: Phase 1 (Move Generation)
The engine currently features a fully functional Pseudo-Legal Move Generator. It processes board states using low-level bitwise operations to generate a complete list of physically possible moves for the active color, encompassing directional pawn pushes, leaper jumps, and blocked sliding raycasts.

## Core Architecture

### 1. Bitboard Representation (`board.cpp` / `board.h`)
* **64-bit State:** The board is represented using arrays of `std::uint64_t` bitboards, separating pieces by color and type.
* **Aggregate Bitboards:** Maintains updated `whitePieces`, `blackPieces`, and `allPieces` bitboards for instant occupancy lookups.
* **Coordinate System:** Follows a standard 0-63 index mapping (a1 = 0, h8 = 63) with upward/rightward growth.

### 2. Leaper Move Generation (`movegen.cpp` / `movegen.h`)
* **Pre-calculated Arrays:** Attack patterns for Knights and Kings are calculated once at startup (`init()`) and stored in lookup tables.
* **Directional Pawn Captures:** Pre-calculated capture tables handle White (Up) and Black (Down) pawn attacks.
* **Edge Wrap Prevention:** Strict mathematical masking (`notAFile`, `notHFile`, etc.) prevents pieces from wrapping around the board edges during bit shifts.

### 3. Sliding Piece Raycasting
* **On-The-Fly Generation:** Utilizes dynamic loops to calculate raycasts for Bishops, Rooks, and Queens (`getRookAttacks`, `getBishopAttacks`).
* **Occupancy Detection:** Rays terminate instantly upon intersecting with the `allPieces` bitboard to handle captures and friendly blockers.
* **Magic Bitboard Preparation:** Includes foundational "Outer Edge Exclusion" masks (`maskRookAttacks`, `maskBishopAttacks`) designed for a future upgrade to Magic Bitboard hashing.

### 4. Hardware-Accelerated Bit Manipulation
* **Trailing Zero Count:** Leverages the `__builtin_ctzll` compiler intrinsic to extract piece locations from bitboards in a single CPU cycle.
* **Brian Kernighan's Algorithm:** Uses `bb &= bb - 1` to systematically clear the least significant bit, allowing loop-free piece extraction.
* **Bitwise Pawn Logic:** Calculates single and double pawn pushes simultaneously using fast bitwise shifts and rank-mask filtering.

## Roadmap

### Engine Core
- [ ] Implement `makeMove()` and `unmakeMove()` to update bitboards dynamically.
- [ ] Implement Legal Move Filtering (preventing moves that leave the King in check).
- [ ] Add special rules: Castling, En Passant, and Pawn Promotion.
- [ ] Develop board evaluation function (material + positional scoring).
- [ ] Implement Minimax search algorithm with Alpha-Beta Pruning.

### User Interface
- [ ] Develop custom Python UI.
- [ ] Bind the C++ engine core to the Python frontend.