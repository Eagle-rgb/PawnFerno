#pragma once

#ifndef BITBOARD_INCLUDE
#define BITBOARD_INCLUDE

#define QUEEN_DIRECTIONS DIRECTIONS

#include "types.h"
#include <cmath>

constexpr BitBoard BB_EMPTY = 0;
constexpr BitBoard BB_FULL = ~BB_EMPTY;

constexpr BitBoard BB_a1 = 1;

constexpr BitBoard BB_RANK1 = 0xFF;
constexpr BitBoard BB_RANK2 = BB_RANK1 << 8;
constexpr BitBoard BB_RANK3 = BB_RANK1 << (8*2);
constexpr BitBoard BB_RANK4 = BB_RANK1 << (8*3);
constexpr BitBoard BB_RANK5 = BB_RANK1 << (8*4);
constexpr BitBoard BB_RANK6 = BB_RANK1 << (8*5);
constexpr BitBoard BB_RANK7 = BB_RANK1 << (8*6);
constexpr BitBoard BB_RANK8 = BB_RANK1 << (8*7);

constexpr BitBoard BB_FILEA = 0x0101010101010101;
constexpr BitBoard BB_FILEB = BB_FILEA << 1;
constexpr BitBoard BB_FILEC = BB_FILEA << 2;
constexpr BitBoard BB_FILED = BB_FILEA << 3;
constexpr BitBoard BB_FILEE = BB_FILEA << 4;
constexpr BitBoard BB_FILEF = BB_FILEA << 5;
constexpr BitBoard BB_FILEG = BB_FILEA << 6;
constexpr BitBoard BB_FILEH = BB_FILEA << 7;

constexpr BitBoard BB_BORDER = BB_FILEA | BB_FILEH | BB_RANK1 | BB_RANK8;

constexpr Direction DIRECTIONS[8] = { EAST, SOUTHEAST, SOUTH, SOUTHWEST, WEST, NORTHWEST, NORTH, NORTHEAST };
constexpr BitBoard DIRECTION_LIMITS[8] = { BB_FILEH, BB_FILEH | BB_RANK1, BB_RANK1, BB_FILEA | BB_RANK1,
	BB_FILEA, BB_FILEA | BB_RANK8, BB_RANK8, BB_FILEH | BB_RANK8 };

constexpr Direction BISHOP_DIRECTIONS[4] = { SOUTHEAST, SOUTHWEST, NORTHWEST, NORTHEAST };
constexpr Direction ROOK_DIRECTIONS[4] = { EAST, SOUTH, WEST, NORTH };

constexpr Direction PAWN_DIRECTIONS[2] = { NORTH, SOUTH };
constexpr Rank PAWN_DOUBLE_GO_FORWARD_ON_THE_CHESSBOARD_NON_EN_PASSANT_FOR_WHITE_AND_BLACK[2] = { RANK2, RANK7 };
constexpr Rank PROMOTION_RANKS[2] = { RANK8, RANK1 };
constexpr Rank ENPASSANT_RANKS[2] = { RANK5, RANK4 };

// Precalculated attack masks for pawns, knights and kings.
extern BitBoard PAWN_PUSHES[2][64];
extern BitBoard PAWN_CAPTURES[2][64];
extern BitBoard KNIGHT_ATTACKS[64];
extern BitBoard KING_ATTACKS[64];

// Precalculated rays.
extern BitBoard RAYS[64][8];

namespace bits
{
	// Used for BitScan
	constexpr int lsb_64_table[64] = {
	   63, 30,  3, 32, 59, 14, 11, 33,
	   60, 24, 50,  9, 55, 19, 21, 34,
	   61, 29,  2, 53, 51, 23, 41, 18,
	   56, 28,  1, 43, 46, 27,  0, 35,
	   62, 31, 58,  4,  5, 49, 54,  6,
	   15, 52, 12, 40,  7, 42, 45, 16,
	   25, 57, 48, 13, 10, 39,  8, 44,
	   20, 47, 38, 22, 17, 37, 36, 26
	};

	extern int ms1bTable[255];

	constexpr int bitScanForward(BitBoard bb) {
		unsigned int folded = 0;
		bb ^= bb - 1;
		folded = (int)bb ^ (bb >> 32);
		return lsb_64_table[folded * 0x78291ACF >> 26];
	}

	// Divide and Conquer approach for determining the MS1B.
	constexpr int bitScanBackward(BitBoard bb) {
		int result = 0;
		if (bb > 0xFFFFFFFF) {
			bb >>= 32;
			result = 32;
		}
		if (bb > 0xFFFF) {
			bb >>= 16;
			result += 16;
		}
		if (bb > 0xFF) {
			bb >>= 8;
			result += 8;
		}
		return result + ms1bTable[bb];
	}

	// Returns AND removes the lsb from the given bitboard (the bitboard WILL be changed).
	constexpr Square popLSB(BitBoard& bb) {
		if (bb == BB_EMPTY)
			return SQNONE;

		Square sq = Square(bitScanForward(bb));
		bb &= (bb - 1);
		return sq;
	}
}

// Generating BitBoards from Squares, Ranks, Files
constexpr BitBoard toBB(Square sq) {
	return BB_a1 << sq;
}
constexpr BitBoard toBB(Rank r) {
	return BB_RANK1 << (8 * r);
}
constexpr BitBoard toBB(File f) {
	return BB_FILEA << f;
}

// Gets First occupied Square on BitBoard
constexpr Square toSquare(BitBoard bb){
	return Square(bits::bitScanForward(bb));
}

constexpr bool isEmpty(BitBoard bb){
	return !bool(bb);
}

// Checks if the given Square is empty.
constexpr bool isFree(BitBoard bb, Square s) {
	return isEmpty(bb & toBB(s));
}

// True if the given bitboard has more than one 1.
constexpr bool more_than_one(BitBoard bb) {
	return !isEmpty(bb & bb - 1);
}

// Shift BitBoard by 1 square
constexpr BitBoard shift(BitBoard b, Direction d) {
	return d > 0 ? b << d : b >> -d;
}

constexpr BitBoard shiftBy(BitBoard b, Direction d, int amount) {
	return d > 0 ? b << (amount * d) : b >> -(amount * d);
}

constexpr BitBoard shift(Square sq, Direction d) { return shift(toBB(sq), d); }
constexpr BitBoard shiftBy(Square sq, Direction d, int amount) { return shiftBy(toBB(sq), d, amount); }

void BitBoardInit();
BitBoard pseudoLegalBishop(Square, BitBoard blockers);
BitBoard pseudoLegalRook(Square, BitBoard blockers);
BitBoard pseudoLegalQueen(Square, BitBoard blockers);
BitBoard tillFirstBlocker(Square, BitBoard blockers, Direction);
BitBoard attacksOf(Square from, BitBoard blockers, Color c, PieceType piece);

/// <summary>
/// Returns a BitBoard containing all attacked squares from our pieces.
/// </summary>
/// <param name="ourPieces">: Our attacking pieces. </param>
/// <param name="blockers">: All blocking pieces. </param>
/// <param name="c">: The attacking player. </param>
BitBoard attacksOfAll(BitBoard ourPieces[6], BitBoard blockers, Color c);

/*#define ENABLE_BITBOARD_OPERATORS_ON(T) \
constexpr BitBoard operator&(BitBoard b, T d) { return BitBoard(toBB(d) & b); }	\
constexpr BitBoard operator|(BitBoard b, T d) { return BitBoard(toBB(d) | b); }	\
constexpr BitBoard operator^(BitBoard b, T d) { return BitBoard(toBB(d) ^ b); }	\
constexpr BitBoard operator&(T d, BitBoard b) { return BitBoard(toBB(d) & b); }	\
constexpr BitBoard operator|(T d, BitBoard b) { return BitBoard(toBB(d) | b); }	\
constexpr BitBoard operator^(T d, BitBoard b) { return BitBoard(toBB(d) ^ b); }	\
constexpr BitBoard operator~(T d) { return ~toBB(d); }							\
inline BitBoard& operator&=(BitBoard& b, T d) { return b = b & d; }				\
inline BitBoard& operator|=(BitBoard& b, T d) { return b = b | d; }				\
inline BitBoard& operator^=(BitBoard& b, T d) { return b = b ^ d; }				

ENABLE_BITBOARD_OPERATORS_ON(Rank)
ENABLE_BITBOARD_OPERATORS_ON(File)
ENABLE_BITBOARD_OPERATORS_ON(Square)

#undef ENABLE_BITBOARD_OPERATORS_ON*/

#endif