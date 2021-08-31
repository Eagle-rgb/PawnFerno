#pragma once
#include "types.h"

constexpr BitBoard BB_EMPTY = 0;
constexpr BitBoard BB_FULL = ~BB_EMPTY;

constexpr BitBoard BB_a1 = 1;

constexpr BitBoard BB_RANK1 = 0xFF;
constexpr BitBoard BB_RANK2 = toBB(RANK2);
constexpr BitBoard BB_RANK3 = toBB(RANK3);
constexpr BitBoard BB_RANK4 = toBB(RANK4);
constexpr BitBoard BB_RANK5 = toBB(RANK5);
constexpr BitBoard BB_RANK6 = toBB(RANK6);
constexpr BitBoard BB_RANK7 = toBB(RANK7);
constexpr BitBoard BB_RANK8 = toBB(RANK8);

constexpr BitBoard BB_FILEA = 0x0101010101010101;
constexpr BitBoard BB_FILEB = toBB(FILEB);
constexpr BitBoard BB_FILEC = toBB(FILEC);
constexpr BitBoard BB_FILED = toBB(FILED);
constexpr BitBoard BB_FILEE = toBB(FILEE);
constexpr BitBoard BB_FILEF = toBB(FILEF);
constexpr BitBoard BB_FILEG = toBB(FILEG);
constexpr BitBoard BB_FILEH = toBB(FILEH);

constexpr Directions PAWN_DIRECTIONS[2] = { NORTH, SOUTH };
constexpr Ranks PAWN_DOUBLE_GO_FORWARD_ON_THE_CHESSBOARD_NON_EN_PASSANT_FOR_WHITE_AND_BLACK[2] = { RANK2, RANK7 };

// Precalculated attack masks for pawns, knights and kings.
BitBoard PAWN_PUSHES[2][64];
BitBoard PAWN_CAPTURES[2][64];
BitBoard KNIGHT_ATTACKS[64];
BitBoard KING_ATTACKS[64];

constexpr void BitBoardInit();

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

// Generating BitBoards from Squares, Ranks, Files
constexpr BitBoard toBB(Squares sq) {
	return BB_a1 << sq;
}
constexpr BitBoard toBB(Ranks r) {
	return BB_RANK1 << (8 * r);
}
constexpr BitBoard toBB(Files f) {
	return BB_FILEA << f;
}

// Get Rank, File from Square
constexpr Ranks toRank(Squares sq){
	return Ranks(sq / 8);
}
constexpr Files toFile(Squares sq){
	return Files(sq % 8);
}

// Gets First occupied Square on BitBoard
constexpr Squares toSquare(BitBoard bb){
	return Squares(bitScanForward(bb));
}
constexpr Squares toSquare(Files f, Ranks r){
	return Squares((8*r) + f);
}

constexpr bool isEmpty(BitBoard bb){
	return !bool(bb);
}

// Shift BitBoard by 1 square
constexpr BitBoard shift(BitBoard b, Directions d) {
	return d > 0 ? b << d : b >> -d;
}

constexpr BitBoard shiftBy(BitBoard b, Directions d, int amount) {
	return d > 0 ? b << (amount * d) : b >> -(amount * d);
}

inline BitBoard operator& (Squares sq, BitBoard b) {
	return toBB(sq) & b;
}

int bitScanForward(BitBoard bb) {
   unsigned int folded;
   bb ^= bb - 1;
   folded = (int) bb ^ (bb >> 32);
   return lsb_64_table[folded * 0x78291ACF >> 26];
}
