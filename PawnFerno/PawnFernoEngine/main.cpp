#include "main.h"

int main() {
	BitBoardInit();

	State a, b, c, d, e;

	Position p("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 0", &a);

	perft(&p, 3);
}