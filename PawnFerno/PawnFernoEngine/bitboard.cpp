#include "bitboard.h"

BitBoard PAWN_PUSHES[2][64];
BitBoard PAWN_CAPTURES[2][64];
BitBoard KNIGHT_ATTACKS[64];
BitBoard KING_ATTACKS[64];

BitBoard RAYS[64][8];

namespace bits {
	int ms1bTable[255];
}

void BitBoardInit() {
	// Initializing msb table.
	bits::ms1bTable[0] = 0;
	for (int i = 0; i < 8; i++)
		for (int j = std::pow(2, i); j <= std::pow(2, i + 1) - 1; j++)
			bits::ms1bTable[j] = i;

	for (Square sq = SQa1; sq <= SQh8; ++sq) {

		BitBoard squareBB = toBB(sq);

		// Pawn moves
		// TODO: change to toRank() == sldkfjal�sf
		if ((BB_RANK1 & squareBB | BB_RANK8 & squareBB) != BB_EMPTY) {
			PAWN_PUSHES[WHITE][sq] = BB_EMPTY;
			PAWN_PUSHES[BLACK][sq] = BB_EMPTY;
			PAWN_CAPTURES[WHITE][sq] = BB_EMPTY;
			PAWN_CAPTURES[BLACK][sq] = BB_EMPTY;
		}
		else {
			for (int i = 0; i <= 1; i++)
			{
				Color c = Color(i);
				PAWN_PUSHES[c][sq] = shift(squareBB, PAWN_DIRECTIONS[c]);
				PAWN_CAPTURES[c][sq] = shift(~BB_FILEH & squareBB, Direction(PAWN_DIRECTIONS[c] + EAST)) |
					shift(~BB_FILEA & squareBB, Direction(PAWN_DIRECTIONS[c] + WEST));

				if (toRank(sq) == PAWN_DOUBLE_GO_FORWARD_ON_THE_CHESSBOARD_NON_EN_PASSANT_FOR_WHITE_AND_BLACK[c]) {
					PAWN_PUSHES[c][sq] |= shiftBy(squareBB, PAWN_DIRECTIONS[c], 2);
				}
			}
		}

		// King moves
		KING_ATTACKS[sq] = squareBB;
		KING_ATTACKS[sq] |= shift((KING_ATTACKS[sq] & ~BB_RANK1), SOUTH);
		KING_ATTACKS[sq] |= shift((KING_ATTACKS[sq] & ~BB_RANK8), NORTH);
		KING_ATTACKS[sq] |= shift((KING_ATTACKS[sq] & ~BB_FILEA), WEST);
		KING_ATTACKS[sq] |= shift((KING_ATTACKS[sq] & ~BB_FILEH), EAST);
		KING_ATTACKS[sq] ^= squareBB;

		// Knight moves
		KNIGHT_ATTACKS[sq] = BB_EMPTY;
		KNIGHT_ATTACKS[sq] |= (~(BB_RANK1 | BB_RANK2 | BB_FILEA) & squareBB) >> 17;
		KNIGHT_ATTACKS[sq] |= (~(BB_RANK1 | BB_FILEA | BB_FILEB) & squareBB) >> 10;
		KNIGHT_ATTACKS[sq] |= (~(BB_RANK8 | BB_FILEA | BB_FILEB) & squareBB) << 6;
		KNIGHT_ATTACKS[sq] |= (~(BB_RANK7 | BB_RANK8 | BB_FILEA) & squareBB) << 15;
		KNIGHT_ATTACKS[sq] |= (~(BB_RANK7 | BB_RANK8 | BB_FILEH) & squareBB) << 17;
		KNIGHT_ATTACKS[sq] |= (~(BB_RANK8 | BB_FILEG | BB_FILEH) & squareBB) << 10;
		KNIGHT_ATTACKS[sq] |= (~(BB_RANK1 | BB_FILEG | BB_FILEH) & squareBB) >> 6;
		KNIGHT_ATTACKS[sq] |= (~(BB_RANK1 | BB_RANK2 | BB_FILEH) & squareBB) >> 15;
	}

	// Calculate the rays.
	for (int i = 0; i < 8; i++) {
		Direction direction = DIRECTIONS[i];

		// To calculate all rays in direction 'direction', we simply accumulate the ray while going
		// in the backwards direction. We use the borders in the direction as initial start squares,
		// as their ray will be BB_EMPTY.
		BitBoard startSquares = DIRECTION_LIMITS[i];

		// (i + 4) & 7 gives the index of the opposite direction.
		BitBoard limits = DIRECTION_LIMITS[(i + 4) & 7];

		Square sq = SQNONE;

		while ((sq = bits::popLSB(startSquares)) != SQNONE)
		{
			BitBoard b = BB_EMPTY;

			while ((toBB(sq) & limits) == BB_EMPTY)
			{
				RAYS[sq][i] = b;
				b |= toBB(sq);
				sq += -direction;
			}

			RAYS[sq][i] = b;
		}
	}
}

BitBoard pseudoLegalBishop(Square from, BitBoard blockers) {
	BitBoard pseudoLegals = BB_EMPTY;

	for (int i = 0; i < 4; i++)
		pseudoLegals |= tillFirstBlocker(from, blockers, BISHOP_DIRECTIONS[i]);

	return pseudoLegals;
}

BitBoard pseudoLegalRook(Square from, BitBoard blockers) {
	BitBoard pseudoLegals = BB_EMPTY;

	for (int i = 0; i < 4; i++)
		pseudoLegals |= tillFirstBlocker(from, blockers, ROOK_DIRECTIONS[i]);

	return pseudoLegals;
}

BitBoard pseudoLegalQueen(Square from, BitBoard blockers) { return pseudoLegalBishop(from, blockers) | pseudoLegalRook(from, blockers); }

BitBoard tillFirstBlocker(Square from, BitBoard blockers, Direction d) {
	BitBoard rayMask = RAYS[from][directionIndex(d)];
	BitBoard blockerCollision = rayMask & blockers;

	if (blockerCollision == BB_EMPTY) return rayMask;

	Square firstCollision = Square(d > 0 ? bits::bitScanForward(blockerCollision) : bits::bitScanBackward(blockerCollision));
	return rayMask ^ RAYS[firstCollision][directionIndex(d)];
}

BitBoard attacksOf(Square from, BitBoard blockers, Color c, PieceType piece) {
	switch (piece)
	{
	case PAWN: return PAWN_CAPTURES[c][from];
	case KNIGHT: return KNIGHT_ATTACKS[from];
	case BISHOP: return pseudoLegalBishop(from, blockers);
	case ROOK: return pseudoLegalRook(from, blockers);
	case QUEEN: return pseudoLegalQueen(from, blockers);
	case KING: return KING_ATTACKS[from];
	default: return BB_EMPTY;
	}
}

BitBoard attacksOfAll(BitBoard ourPieces[6], BitBoard blockers, Color c) {
	BitBoard result = BB_EMPTY;

	for (PieceType p = PAWN; p <= KING; ++p) {
		Square sq = SQNONE;

		while ((sq = bits::popLSB(ourPieces[p])) != SQNONE) {
			result |= attacksOf(sq, blockers, c, p);
		}
	}

	return result;
}

