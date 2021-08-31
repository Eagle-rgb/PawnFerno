#include "bitboard.h"

constexpr void BitBoardInit() {
	for (Squares sq = SQa1; sq <= SQh8; sq++) {

		// TODO: change to toRank() == sldkfjalösf
		if ((sq & BB_RANK1 | sq & BB_RANK8) != BB_EMPTY) {
			PAWN_PUSHES[WHITE][sq] = BB_EMPTY;
			PAWN_PUSHES[BLACK][sq] = BB_EMPTY;
			continue;
		}

		for (Colors c = WHITE; c <= BLACK; c = Colors(c + 1))
		{
			PAWN_PUSHES[c][sq] = shift(toBB(sq), PAWN_DIRECTIONS[c]);

			if (toRank(sq) == PAWN_DOUBLE_GO_FORWARD_ON_THE_CHESSBOARD_NON_EN_PASSANT_FOR_WHITE_AND_BLACK[c]) {
				PAWN_PUSHES[c][sq] |= shiftBy(toBB(sq), PAWN_DIRECTIONS[c], 2);
			}
		}

	}
}
