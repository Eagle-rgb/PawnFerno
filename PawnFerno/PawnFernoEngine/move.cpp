#include "move.h"

namespace move {
	std::vector<Move> makeNormalMoves(Square from, BitBoard destinations) {
		std::vector<Move> moves;

		while (destinations) {
			moves.push_back(makeMove(from, bits::popLSB(destinations)));
		}

		return moves;
	}

	std::vector<Move> makePromotions(Square from, BitBoard destinations) {
		std::vector<Move> moves;

		while (destinations) {
			Square dest = bits::popLSB(destinations);
			for (PieceType p = KNIGHT; p <= QUEEN; ++p)
				moves.push_back(makePromotionMove(from, dest, p));
		}

		return moves;
	}

}