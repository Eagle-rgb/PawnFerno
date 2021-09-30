#pragma once

#ifndef SEARCH_INCLUDED
#define SEARCH_INCLUDED

#include "position.h"
#include "ioutil.h"
#include "evaluation.h"
#include <random>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

namespace n_search {
	using Milliseconds = typename std::chrono::milliseconds;
	using Seconds = typename std::chrono::seconds;
	using TimePoint = typename std::chrono::steady_clock::time_point;
	using Clock = typename std::chrono::steady_clock;

	enum class SearchType {
		PERFT, NORMAL
	};

	struct SearchOption {
		SearchType m_searchType;
		int m_depth;
	};

	struct SearchResult {
		unsigned int m_nodes;
		int m_score = 0;
		Move m_move;

		SearchResult& operator+=(SearchResult& res);
		inline bool operator<(SearchResult& other) { return m_score < other.m_score; }
		inline bool operator>(SearchResult& other) { return m_score > other.m_score; }
		inline bool operator<=(SearchResult& other) { return m_score <= other.m_score; }
		inline bool operator>=(SearchResult& other) { return m_score >= other.m_score; }
		inline SearchResult& operator-() { m_score = -m_score; return *this; }
	};

	struct Search {
		Position* pos;
		SearchOption option;
		std::vector<SearchResult> results;
		SearchResult bestResult;

		TimePoint t_begin, t_end;

		Search(Search&) = delete;
		Search(Position* pos, SearchOption& option) {
			this->pos = pos; this->option = option;
		}

		unsigned int totalNodes();

		template <typename Unit = Seconds> unsigned long long totalTime();
		template <> unsigned long long totalTime<Seconds>();
		template <> unsigned long long totalTime<Milliseconds>();
	};

	std::ostream& operator<<(std::ostream& stream, Search& search);

	const SearchResult EMPTYRESULT = SearchResult{ 0, eval::VALUE_MATE - 100, 0 };
	const SearchResult ONERESULT = SearchResult{ 1, eval::VALUE_MATE - 100, 0 };

	void start(Position* pos, SearchOption& option);
}

#endif