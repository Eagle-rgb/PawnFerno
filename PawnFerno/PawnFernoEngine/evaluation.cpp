#include "evaluation.h"

namespace eval {

	Score SQUARE_VALUES[2][6][64];

	void init() {
		for (Square sq = SQa1; sq <= SQh8; ++sq) {
			SQUARE_VALUES[WHITE][PAWN][sq] = SQVALUES_PAWN[sq];
			SQUARE_VALUES[BLACK][PAWN][sq] = SQVALUES_PAWN[relativeSquare(sq, BLACK)];
			
			SQUARE_VALUES[WHITE][KNIGHT][sq] = SQVALUES_KNIGHT[sq];
			SQUARE_VALUES[BLACK][KNIGHT][sq] = SQVALUES_KNIGHT[relativeSquare(sq, BLACK)];

			SQUARE_VALUES[WHITE][BISHOP][sq] = SQVALUES_BISHOP[sq];
			SQUARE_VALUES[BLACK][BISHOP][sq] = SQVALUES_BISHOP[relativeSquare(sq, BLACK)];

			SQUARE_VALUES[WHITE][ROOK][sq] = SQVALUES_ROOK[sq];
			SQUARE_VALUES[BLACK][ROOK][sq] = SQVALUES_ROOK[relativeSquare(sq, BLACK)];

			SQUARE_VALUES[WHITE][QUEEN][sq] = SQVALUES_QUEEN[sq];
			SQUARE_VALUES[BLACK][QUEEN][sq] = SQVALUES_QUEEN[relativeSquare(sq, BLACK)];

			SQUARE_VALUES[WHITE][KING][sq] = SQVALUES_KING[sq];
			SQUARE_VALUES[BLACK][KING][sq] = SQVALUES_KING[relativeSquare(sq, BLACK)];
		}
	}
}