#include "hashing.h"

namespace hashing {
	HashKey HASH_SQUARE_KEYS[64][16];

	HashKey PseudoRandomGenerator::getNext() {
		return current = (current << 28) ^ (current >> 16);
	}

	void init() {

	}
}
