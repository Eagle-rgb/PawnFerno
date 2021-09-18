#include "main.h"

int main() {
	BitBoardInit();
	State s = startState();
	State n = startState();

	Position p = Position(startPosFen, &s);

	std::cout << p.print();

	p.makeMove(move::makeMove(SQe2, SQe4), n);

	std::cout << std::endl << p.print();

	p.undoMove(move::makeMove(SQe2, SQe4));
	std::cout << std::endl << p.print();
}