#include "hashing.h"

namespace hashing {
	HashKey SQUARE_KEYS[64][12];
	HashKey COLORKEY;
	HashKey ENPASSANT_KEYS[8];
	HashKey CASTLING_KEYS[15];

	void init() {
		for (int i = 0; i < 64; ++i) {
			for (int j = 0; j < 12; ++j) {
				SQUARE_KEYS[i][j] = rand() | ((uint64_t) rand() << 32);
			}
		}

		COLORKEY = rand() | ((uint64_t)rand() << 32);

		for (int i = 0; i < 8; ++i) {
			ENPASSANT_KEYS[i] = rand() | ((uint64_t)rand() << 32);
		}

		for (int i = 0; i < 15; ++i) {
			CASTLING_KEYS[i] = rand() | ((uint64_t)rand() << 32);
		}
	}

	HashKey squareKey(Square sq, Piece p) {
		return SQUARE_KEYS[sq][p];
	}

	HashKey enpassantKey(File f) {
		return ENPASSANT_KEYS[f];
	}

	HashKey castlingKey(short c) {
		return CASTLING_KEYS[c];
	}

	// PRNG
	HashKey PRNG::getNext() {
		return current = (current << 28) ^ (current >> 16);
	}

	// Zobrist

	// HashTable
	template <>
	bool HashTable<ALWAYS>::add(SearchNode data, HashKey key) {
		bool active = (*this)[key].active;

		(*this)[key] = data;
		return active;
	}

	template <>
	bool HashTable<DEPTHHIGH>::add(SearchNode data, HashKey key) {
		auto& entry = (*this)[key];

		if (!entry.active) {
			(*this)[key] = data;
			return false;
		}

		// active. Check if this depth is higher
		if (entry.depth <= data.depth) {
			(*this)[key] = data;
			return true;
		}

		return false;
	}
}
