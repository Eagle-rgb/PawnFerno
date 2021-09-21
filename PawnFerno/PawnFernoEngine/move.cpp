#include "move.h"

namespace move {
	std::vector<Move> makeNormalMoves(Square from, BitBoard destinations) {
		Square sq = SQNONE;
		std::vector<Move> moves;

		while ((sq = bits::popLSB(destinations)) != SQNONE) {
			moves.push_back(makeMove(from, sq));
		}

		return moves;
	}

	std::vector<Move> makePromotions(Square from, BitBoard destinations) {
		Square sq = SQNONE;
		std::vector<Move> moves;

		while ((sq = bits::popLSB(destinations)) != SQNONE) {
			for (PieceType p = KNIGHT; p <= QUEEN; ++p) {
				moves.push_back(makePromotionMove(from, sq, p));
			}
		}

		return moves;
	}

}