#include "main.h"

int main() {
	BitBoardInit();
	State s, n, nn;

	std::string fen = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 0";

	Position p = Position(fen, &s);

	std::cout << std::endl << p.print();
	
	std::vector<Move> moves = p.getLegalMovesAuto();

	p.makeMove(moves[1], n);

	std::cout << std::endl << p.print();

	std::vector<Move> moves2 = p.getLegalMovesAuto();

	p.makeMove(moves2[1], nn);

	std::cout << std::endl << p.print();

	p.undoMove(moves2[1]); 
	
	std::cout << std::endl << p.print(); 
	
	p.undoMove(moves[1]);

	std::cout << std::endl << p.print();
}