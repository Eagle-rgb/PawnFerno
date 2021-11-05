#include "search.h"

namespace n_search {
	hashing::HashTable<hashing::DEPTHHIGH> ttTable(1000000UL);

	SearchResult search(Search* s, int depth, int alpha, int beta, int colorCoeff) {
		Position& pos = *(s->pos);
		SearchResult res = EMPTYRESULT;
		res.m_score = alpha;

		if (depth == s->option.m_depth) {
			pos.getLegalMovesAuto();  // to get mating status.
			res.m_nodes = 1;
			res.m_score = colorCoeff * s->pos->static_eval();
			return res;
		}

		/*hashing::SearchNode& ttHit = ttTable.get(pos.getKey());  // Try to get a hit.
		if (ttHit) {
			if (ttHit.depth < depth) {
				res.m_nodes = 1; res.m_score = ttHit.posScore; res.m_move = ttHit.bestMove; res;
			}
		}*/

		auto moveList = pos.getLegalMovesAuto();
		pos.sortMoves(moveList);  // circa 25% speedup!
		Score bestScore = eval::VALUE_MATE - 1;

		for (Move move : moveList) {
			State a;
			pos.makeMove(move, a); 

			SearchResult recResult = -search(s, depth + 1, -beta, -alpha, -colorCoeff);
			recResult.m_move = move;

			res.m_nodes += recResult.m_nodes;

			if (recResult.m_score > bestScore) {
				bestScore = recResult.m_score;
				res.m_score = bestScore;
				res.m_move = move;

				alpha = res.m_score;
			}

			if (alpha > beta) {
				res.m_score = beta;  // We will never take this result, so just set it to the highest value, and by returning it, the lowest.
				pos.undoMove(move);
				return res;
			}

			pos.undoMove(move);
		}

		return res;
	}

	SearchResult perft(Search* s, int depth) {
		if (depth == s->option.m_depth) {
			return ONERESULT;
		}

		auto moveList = s->pos->getLegalMovesAuto();
		SearchResult total = EMPTYRESULT;

		for (Move move : moveList) {
			State a;
			s->pos->makeMove(move, a);
			total.m_nodes += perft(s, depth + 1).m_nodes;
			s->pos->undoMove(move);
		}

		return total;
	}

	SearchResult searchTemp(Search* s, int depth, bool root = false) {
		auto moveList = s->pos->getLegalMovesAuto();

		unsigned short randNum = std::rand() % moveList.size();
		s->pos->makeMoveForgetful(moveList[randNum]);
		s->bestResult.m_move = moveList[randNum];

		return SearchResult();
	}

	void start(Position* pos, SearchOption& option) {
		Search s(pos, option);
		s.t_begin = Clock::now();

		auto moveList = pos->getLegalMovesAuto();

		s.bestResult = EMPTYRESULT;
		int alpha = eval::VALUE_MATE;
		int beta = -eval::VALUE_MATE;

		int colorCoeff = pos->playerToMove() ? 1 : -1;

		for (Move move : moveList) {
			State a;
			
			pos->makeMove(move, a);
			SearchResult recResult;
			
			if (option.m_searchType == SearchType::PERFT) {
				recResult = perft(&s, 1);
			}
			else {
				recResult = -search(&s, 1, -beta, -alpha, colorCoeff);
			}

			std::cout << "info " << printing::print(move) << ": " << recResult.m_score << std::endl;

			recResult.m_move = move;

			if (recResult > s.bestResult) {
				s.bestResult = recResult;
				alpha = recResult.m_score;
			}

			s.results.push_back(recResult);

			pos->undoMove(move);
		}

		s.t_end = Clock::now();
		std::cout << s;

		if (option.m_searchType != SearchType::PERFT) {
			if (s.bestResult.m_move != 0) {
				std::cout << "bestmove " << printing::print(s.bestResult.m_move) << "\n";
			}
			else std::cout << "info No move could be found.\n";
		}
	}

	unsigned int Search::totalNodes() {
		unsigned int total = 0;

		for (const SearchResult& sr : results) {
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
			for (const SearchResult& res : search.results) {
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