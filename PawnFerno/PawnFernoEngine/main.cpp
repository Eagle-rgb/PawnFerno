#include "main.h"

int main() {
	BitBoardInit();

	Position p(startPosFen);

	std::cout << p.print();

	

	p.makeMove(move::makeMove(SQe2, SQe4));

	std::cout << std::endl << p.print();
}