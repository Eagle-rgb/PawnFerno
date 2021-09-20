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

}