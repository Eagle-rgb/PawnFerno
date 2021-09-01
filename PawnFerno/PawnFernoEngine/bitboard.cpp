#include "bitboard.h"

BitBoard PAWN_PUSHES[2][64];
BitBoard PAWN_CAPTURES[2][64];
BitBoard KNIGHT_ATTACKS[64];
BitBoard KING_ATTACKS[64];

BitBoard RAYS[64][8];

void BitBoardInit() {
	for (Square sq = SQa1; sq <= SQh8; ++sq) {

		// Pawn moves
		// TODO: change to toRank() == sldkfjal�sf
		if ((BB_RANK1 & sq | BB_RANK8 & sq) != BB_EMPTY) {
			PAWN_PUSHES[WHITE][sq] = BB_EMPTY;
			PAWN_PUSHES[BLACK][sq] = BB_EMPTY;
			PAWN_CAPTURES[WHITE][sq] = BB_EMPTY;
			PAWN_CAPTURES[BLACK][sq] = BB_EMPTY;
			continue;
		}

		for (Color c = WHITE; c <= BLACK; ++c)
		{
			PAWN_PUSHES[c][sq] = shift(sq, PAWN_DIRECTIONS[c]);
			PAWN_CAPTURES[c][sq] = shift(~BB_FILEH & sq, Direction(PAWN_DIRECTIONS[c] + EAST)) |
				shift(~BB_FILEA & sq, Direction(PAWN_DIRECTIONS[c] + WEST));

			if (toRank(sq) == PAWN_DOUBLE_GO_FORWARD_ON_THE_CHESSBOARD_NON_EN_PASSANT_FOR_WHITE_AND_BLACK[c]) {
				PAWN_PUSHES[c][sq] |= shiftBy(sq, PAWN_DIRECTIONS[c], 2);
			}
		}

		// King moves
		KING_ATTACKS[sq] = toBB(sq);
		KING_ATTACKS[sq] |= shift((KING_ATTACKS[sq] & ~BB_RANK1), SOUTH);
		KING_ATTACKS[sq] |= shift((KING_ATTACKS[sq] & ~BB_RANK8), NORTH);
		KING_ATTACKS[sq] |= shift((KING_ATTACKS[sq] & ~BB_FILEA), WEST);
		KING_ATTACKS[sq] |= shift((KING_ATTACKS[sq] & ~BB_FILEH), EAST);
		KING_ATTACKS[sq] ^= sq;

		// Knight moves
		KNIGHT_ATTACKS[sq] = BB_EMPTY;
		KNIGHT_ATTACKS[sq] |= (~(BB_RANK1 | BB_RANK2 | BB_FILEA) & sq) >> 17;
		KNIGHT_ATTACKS[sq] |= (~(BB_RANK1 | BB_FILEA | BB_FILEB) & sq) >> 10;
		KNIGHT_ATTACKS[sq] |= (~(BB_RANK8 | BB_FILEA | BB_FILEB) & sq) << 6;
		KNIGHT_ATTACKS[sq] |= (~(BB_RANK7 | BB_RANK8 | BB_FILEA) & sq) << 15;
		KNIGHT_ATTACKS[sq] |= (~(BB_RANK7 | BB_RANK8 | BB_FILEH) & sq) << 17;
		KNIGHT_ATTACKS[sq] |= (~(BB_RANK8 | BB_FILEG | BB_FILEH) & sq) << 10;
		KNIGHT_ATTACKS[sq] |= (~(BB_RANK1 | BB_FILEG | BB_FILEH) & sq) >> 6;
		KNIGHT_ATTACKS[sq] |= (~(BB_RANK1 | BB_RANK2 | BB_FILEH) & sq) >> 15;
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

		while ((sq = popLSB(startSquares)) != SQNONE)
		{
			BitBoard b = BB_EMPTY;

			while ((toBB(sq) & limits) == BB_EMPTY)
			{
				RAYS[sq][i] = b;
				b |= sq;
				sq += -direction;
			}

			RAYS[sq][i] = b;
		}
	}
}

