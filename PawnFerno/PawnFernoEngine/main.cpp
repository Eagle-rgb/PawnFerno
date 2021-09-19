#include "main.h"

int main() {
	BitBoardInit();
	State s, n;

	std::string fen = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -";

	Position p = Position(fen, &s);
	
	std::cout << p.print();

	std::cout << std::endl << p.state->castlingRights;
	std::cout << std::endl << p.state->enPassant;

	std::cout << std::endl << printing::print(p.getEnemyAttacks());

	std::cout << std::endl << printing::print(RAYS[SQc3][SOUTHWEST]);

	std::cout << std::endl << p.inCheck();

	std::cout << std::endl << printing::print(RAYS[SQc5][SOUTHEAST]);

	p.makePinnersAndCheckers();

	std::cout << std::endl << printing::print(p.state->pinnedPieces);

	p.getLegalMoves();

	std::cout << std::endl << p.state->legalMoves.size();
}