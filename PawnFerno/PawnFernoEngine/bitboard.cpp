#include "bitboard.h"

BitBoard PAWN_PUSHES[2][64];
BitBoard PAWN_CAPTURES[2][64];
BitBoard KNIGHT_ATTACKS[64];
BitBoard KING_ATTACKS[64];

BitBoard RAYS[64][8];

int SQUARE_DISTANCES[64][64];
Direction RELATIVE_DIRECTION[64][64];
BitBoard BB_LINES[64][64];
BitBoard BB_FORWARD[2][64];
BitBoard FORWARD_SPAN[2][64];
BitBoard FORWARD_DOUBLE_SQUARES[2][64];

namespace bits {
	int ms1bTable[255];
}

// Helper functions.
static BitBoard forward(const Color who, const Square sq) {
	return who ? toBB(sq) - 1 & ~toBB(toRank(sq)) : (BB_FULL - toBB(sq) + 1 & ~toBB(toRank(sq)));
}

static BitBoard forward_span(const Color who, const Square sq) {
	BitBoard forwardMask = FORWARD_SPAN[who][sq];
	File sqFile = toFile(sq);
	BitBoard fileMask = toBB(file_left(sqFile)) | toBB(sqFile) | toBB(file_right(sqFile));
	return forwardMask & fileMask;
}

static BitBoard pawn_pushes(const Color who, const Square pawn) {
	BitBoard result = shift(pawn, PAWN_DIRECTIONS[who]);

	if (canDoublePush(who, pawn))
		result |= shiftBy(pawn, PAWN_DIRECTIONS[who], 2);

	return result;
}

void BitBoardInit() {
	// Initializing msb table.
	bits::ms1bTable[0] = 0;
	for (int i = 0; i < 8; i++)
		for (int j = std::pow(2, i); j <= std::pow(2, i + 1) - 1; j++)
			bits::ms1bTable[j] = i;

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

	for (Square sq = SQa1; sq <= SQh8; ++sq) {

		BitBoard squareBB = toBB(sq);

		// spans and forwardbb
		for (int i = 0; i <= 1; ++i) {
			Color c = Color(i);

			BB_FORWARD[c][sq] = forward(c, sq);
			FORWARD_SPAN[c][sq] = forward_span(c, sq);

			Rank sqRank = toRank(sq);
			FORWARD_DOUBLE_SQUARES[c][sq] = c ? (squareBB << (sqRank + 1)) | (squareBB << (sqRank + 2)) :
				(squareBB >> (sqRank + 1)) | (squareBB >> (sqRank + 2));
		}

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
				PAWN_CAPTURES[c][sq] = shift(~BB_FILEH & squareBB, Direction(PAWN_DIRECTIONS[c] + EAST)) |
					shift(~BB_FILEA & squareBB, Direction(PAWN_DIRECTIONS[c] + WEST));
				
				PAWN_PUSHES[c][sq] = pawn_pushes(c, sq);
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

		// Distances and directions.
		for (Square sq2 = SQa1; sq2 <= SQh8; ++sq2) {
			SQUARE_DISTANCES[sq][sq2] = std::max(distance<Rank>(sq, sq2), distance<Rank>(sq, sq2));

			int x = toFile(sq2) > toFile(sq) ? 1 : (toFile(sq2) < toFile(sq) ? -1 : 0);
			int y = toRank(sq2) > toRank(sq) ? 8 : (toRank(sq2) < toRank(sq) ? -8 : 0);
			RELATIVE_DIRECTION[sq][sq2] = Direction(x + y);

			Direction relDir = RELATIVE_DIRECTION[sq][sq2];
			BB_LINES[sq][sq2] = RAYS[sq][directionIndex(relDir)] & RAYS[sq2][directionIndex(-relDir)];
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

BitBoard attacksOfAll(const Square pieceSquares[12][16], BitBoard blockers, Color c) {
	BitBoard result = BB_EMPTY;

	for (PieceType p = PAWN; p <= KING; ++p) {
		Piece piece = makeColoredPiece(p, c);

		for (int i = 0; pieceSquares[piece][i] != SQNONE; ++i) {
			Square sq = pieceSquares[piece][i];

			result |= attacksOf(sq, blockers, c, p);
		}
	}

	return result;
}

