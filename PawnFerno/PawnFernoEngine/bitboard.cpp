#include "bitboard.h"

void BitBoardInit() {
	for (Squares sq = SQa1; sq <= SQh8; sq++) {

		// Pawn moves
		// TODO: change to toRank() == sldkfjal�sf
		if ((sq & BB_RANK1 | sq & BB_RANK8) != BB_EMPTY) {
			PAWN_PUSHES[WHITE][sq] = BB_EMPTY;
			PAWN_PUSHES[BLACK][sq] = BB_EMPTY;
			PAWN_CAPTURES[WHITE][sq] = BB_EMPTY;
			PAWN_CAPTURES[BLACK][sq] = BB_EMPTY;
			continue;
		}

		for (Colors c = WHITE; c <= BLACK; c = Colors(c + 1))
		{
			PAWN_PUSHES[c][sq] = shift(toBB(sq), PAWN_DIRECTIONS[c]);
			PAWN_CAPTURES[c][sq] = shift(toBB(sq), Directions(PAWN_DIRECTIONS[c] + EAST)) |
				shift(toBB(sq), Directions(PAWN_DIRECTIONS[c] + WEST));

			if (toRank(sq) == PAWN_DOUBLE_GO_FORWARD_ON_THE_CHESSBOARD_NON_EN_PASSANT_FOR_WHITE_AND_BLACK[c]) {
				PAWN_PUSHES[c][sq] |= shiftBy(toBB(sq), PAWN_DIRECTIONS[c], 2);
			}
		}

		// King moves
		KING_ATTACKS[sq] = toBB(sq);
		KING_ATTACKS[sq] |= shift((KING_ATTACKS[sq] & ~BB_RANK1), SOUTH);
		KING_ATTACKS[sq] |= shift((KING_ATTACKS[sq] & ~BB_RANK8), NORTH);
		KING_ATTACKS[sq] |= shift((KING_ATTACKS[sq] & ~BB_FILEA), WEST);
		KING_ATTACKS[sq] |= shift((KING_ATTACKS[sq] & ~BB_FILEH), EAST);
		KING_ATTACKS[sq] ^= sq;
	}
}