#pragma once
#ifndef HASHING_INCLUDED
#define HASHING_INCLUDED

#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <random>
#include "types.h"

namespace hashing {
	extern HashKey SQUARE_KEYS[64][12];
	extern HashKey COLORKEY;
	extern HashKey ENPASSANT_KEYS[8];
	extern HashKey CASTLING_KEYS[15];

	void init();

	HashKey squareKey(Square, Piece);
	HashKey enpassantKey(File);
	HashKey castlingKey(short);

	enum ReplacementScheme {
		ALWAYS, DEPTHHIGH
	};

	class PRNG {
		HashKey current;

	public:
		PRNG(HashKey seed);
		void setSeed(HashKey seed);
		HashKey getNext();
	};

	struct Zobrist {
		unsigned long long key;
	};

	struct SearchNode {
		HashKey posKey;
		Score posScore;
		Score alpha;
		Score beta;
		int8_t depth;
		Move bestMove;

		bool active;

		inline operator bool() {
			return active;
		}
	};

	template <ReplacementScheme R>
	class HashTable {
		uint64_t tableSize;
		SearchNode* table;

		SearchNode& lookup(HashKey key);

		// Operators.
		// Lookup
		inline SearchNode& operator[](HashKey& key) {
			return table[key % tableSize];
		}

	public:
		HashTable(const uint64_t sizeKB);
		~HashTable();

		/// <summary>
		/// Adds the given node to the hashtable. Complies to the set ReplacementScheme. Returns true if an active value has been replaced.
		/// </summary>
		bool add(SearchNode data, HashKey key);
		SearchNode& get(HashKey key);

		constexpr uint64_t getSize() { return tableSize; }
	};

	// HashTable
	template <ReplacementScheme R>
	HashTable<R>::HashTable(const uint64_t sizeKB) {
		this->tableSize = sizeKB * 1000 / sizeof (SearchNode);
		table = new SearchNode[tableSize];
	}

	template <ReplacementScheme R>
	HashTable<R>::~HashTable() {
		delete[] table;
	}

	template <ReplacementScheme R>
	SearchNode& HashTable<R>::lookup(HashKey key) {
		return table[key % tableSize];
	}

	template <ReplacementScheme R>
	SearchNode& HashTable<R>::get(HashKey key) {
		return (*this)[key];
	}
}
#endif