#pragma once
#ifndef HASHING_INCLUDED
#define HASHING_INCLUDED

#include <cmath>
#include <cstdint>
#include <cstdlib>
#include "types.h"

namespace hashing {
	extern HashKey HASH_SQUARE_KEYS[64][16];

	void init();

	class PseudoRandomGenerator {
		HashKey current;

	public:
		PseudoRandomGenerator(HashKey seed);
		void setSeed(HashKey seed);
		HashKey getNext();
	};

	struct Zobrist {
		unsigned long long key;
	};
}
#endif