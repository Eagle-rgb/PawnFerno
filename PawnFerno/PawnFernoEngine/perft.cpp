#include "perft.h"

void perft(std::string fen, int depth) {
	State s;
	Position p(fen, &s);

	int nodes = perftRec(&p, depth, true);

	std::cout << std::endl << std::endl << "Total nodes: " << nodes;
}

void perft(Position* p, int depth) {
	int nodes = perftRec(p, depth, true);

	std::cout << std::endl << std::endl << "Total nodes: " << nodes;
}

int perftRec(Position* p, int depth, bool first) {
	if (depth == 0) return 1;
	std::vector<Move> legalMoves = p->getLegalMovesAuto();
	std::vector<Move>::iterator it;
	State n;

	int totalNodes = 0;

	for (it = legalMoves.begin(); it != legalMoves.end(); ++it) {
		Move m = *it;

		p->makeMove(m, n);
		int nodes = perftRec(p, depth - 1, false);
		p->undoMove(m);

		totalNodes += nodes;

		if (first) {
			std::cout << printing::print(m) << ": " << nodes << std::endl;
		}
	}

	return totalNodes;
}
