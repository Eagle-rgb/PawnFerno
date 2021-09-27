#include "uci.h"

using namespace std;
using namespace n_search;

void go(istringstream& ss, Position& pos, SearchOption& option) {
	string token;

	option.m_searchType = SearchType::NORMAL;

	while (ss >> token) {
		if (token == "perft") { option.m_searchType = SearchType::PERFT; ss >> option.m_depth; }
		else if (token == "depth") { ss >> option.m_depth; }
	}

	n_search::start(&pos, option);
}

void ucicmd() {
	std::cout << uci::engine_identification
		<< "\nuciok\n";
}

void makeMove(string m, Position& pos) {
	Move k = move::makeMove(m);
	k = pos.makeLegalFromPseudo(k);

	if (k == 0) { cout << "info The given move was illegal!\n\n"; return; }
	pos.makeMoveForgetful(k);
}

void uci::uci_loop() {
	State state;
	Position pos(startPosFen, &state);
	SearchOption option;

	std::string cmd, token;

	while (token != "quit") {
		if (!getline(cin, cmd)) cmd = "quit";

		istringstream ss(cmd);

		ss >> skipws >> token;

		if (token == "go") go(ss, pos, option);
		else if (token == "uci") ucicmd();
		else if (token == "isready") cout << "readyok\n";
		else if (token == "print") cout << pos.print() << "\n";
		else if (move::isAlgebraic(token)) makeMove(token, pos);
	}
}