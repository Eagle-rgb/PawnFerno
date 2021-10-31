#include "uci.h"

using namespace std;
using namespace n_search;

void go(istringstream& ss, Position& pos, SearchOption& option) {
	string token;

	option.m_searchType = SearchType::NORMAL;
	option.m_depth = 4;

	while (ss >> token) {
		if (token == "perft") { option.m_searchType = SearchType::PERFT; ss >> option.m_depth; }
		else if (token == "depth") { ss >> option.m_depth; }
	}

	n_search::start(&pos, option);

	(void)0;
}

void position(istringstream& ss, Position& pos, std::vector<Move>& movesPlayed, State* state, bool newGame) {
	string token;

	if (newGame) {
		movesPlayed.clear();
	}

	ss >> token;

	if (token == "fen" && newGame) {
		std::vector<string> fenParts;

		while (ss >> token) {
			if (token == "moves") break;
			fenParts.push_back(token);
		}

		pos = Position(fenParts, state);
	}
	else if (token == "startpos" && newGame) {
		pos = Position(startPosFen, state);
		ss >> token;
	}
	else ss >> skipws >> token;

	if (token != "moves") return;

	int moveCount = movesPlayed.size();
	int i = 0;

	while (ss >> token) {
		if (i++ < moveCount) continue;
		if (!move::isAlgebraic(token)) { cout << "Invalid moves!\n"; return; }
		Move m = move::makeMove(token);
		movesPlayed.push_back(m);

		m = pos.makeLegalFromPseudo(m);
		pos.makeMoveForgetful(m);
	}

	(void)0;
}

void ucicmd() {
	std::cout << uci::engine_identification
		<< "\nuciok\n";

	(void)0;
}

void makeMove(string m, Position& pos) {
	Move k = move::makeMove(m);
	k = pos.makeLegalFromPseudo(k);

	if (k == 0) { cout << "info The given move was illegal!\n\n"; return; }
	pos.makeMoveForgetful(k);

	(void)0;
}

void uci::uci_loop() {
	State state;
	Position pos(startPosFen, &state);
	SearchOption option;

	std::string cmd, token;
	std::vector<Move> movesPlayed;

	bool newGame = true;

	do {
		if (!getline(cin, cmd)) cmd = "quit";

		istringstream ss(cmd);

		ss >> skipws >> token;

		if (token == "go") go(ss, pos, option);

		else if (token == "position") {
			position(ss, pos, movesPlayed, &state, newGame); newGame = false;
		}
		else if (token == "uci") ucicmd();
		else if (token == "isready") cout << "readyok\n";
		else if (token == "ucinewgame") newGame = true;
		else if (token == "fen") cout << "info fen " << pos.toFen() << "\n";
		else if (token == "print") cout << pos.print() << "\n";
		else if (token == "key") cout << "info key " << pos.getKey() << "\n";
		else if (move::isAlgebraic(token)) makeMove(token, pos);
		else cout << "Invalid command!\n";
	} while (token != "quit");

	(void)0;
}