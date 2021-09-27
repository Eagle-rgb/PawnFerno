#include "search.h"

namespace n_search {
	SearchResult search(Search* s, int depth, bool root = false) {
		Position& pos = *(s->pos);
		SearchResult res = emptyResult;

		if (depth == s->option.m_depth) {
			res.m_nodes = 1;
			return res ;
		}

		auto moveList = pos.getLegalMovesAuto();

		for (Move move : moveList) {
			State a;
			pos.makeMove(move, a); 

			SearchResult recResult = search(s, depth + 1);
			recResult.m_move = move;

			if (root) {
				s->results.push_back(recResult);
			}
			else {
				res.m_nodes += recResult.m_nodes;
			}

			pos.undoMove(move);
		}

		return res;
	}

	void searchTemp(Search* s, int depth, bool root = false) {
		auto moveList = s->pos->getLegalMovesAuto();

		unsigned short randNum = std::rand() % moveList.size();
		s->pos->makeMoveForgetful(moveList[randNum]);
		s->bestMove = moveList[randNum];
	}

	void start(Position* pos, SearchOption& option) {
		Search s(pos, option);
		s.t_begin = Clock::now();

		switch (option.m_searchType)
		{
		case SearchType::PERFT: search(&s, 0, true);
		case SearchType::NORMAL: searchTemp(&s, 0, true);
		default:
			break;
		}

		s.t_end = Clock::now();
		std::cout << s;
		std::cout << "bestmove " << printing::print(s.bestMove) << "\n";
	}

	unsigned int Search::totalNodes() {
		unsigned int total = 0;

		for (auto sr : results) {
			total += sr.m_nodes;
		}

		return total;
	}

	template <> unsigned long long Search::totalTime<Seconds>() {
		return std::chrono::duration_cast<Seconds>(t_end - t_begin).count();
	}

	template <> unsigned long long Search::totalTime<Milliseconds>() {
		return std::chrono::duration_cast<Milliseconds>(t_end - t_begin).count();
	}

	SearchResult& SearchResult::operator+=(SearchResult& res) {
		m_nodes += res.m_nodes;
		
		return *this;
	}

	std::ostream& operator<<(std::ostream& stream, Search& search) {
		Color player = search.pos->playerToMove();
		std::string result = "";

		if (search.option.m_searchType == SearchType::PERFT) {
			for (SearchResult res : search.results) {
				result += "info " + printing::print(res.m_move, player) + ": " + std::to_string(res.m_nodes) + "\n";
			}

			stream << result << std::endl;
		}

		stream << "info Total time: " << search.totalTime() << " [s]\n";
		stream << "info Total nodes: " << search.totalNodes() << "\n";
		if (search.totalTime() > 0) stream << "info NPS: " << search.totalNodes() / search.totalTime() << " [n/s]\n";

		return stream;
		std::string searchPrint = "info depth: " + search.option.m_depth;
	}
}