#include "bitboard.h"

BitBoard toBB(Squares sq) {
	return BB_a1 << sq;
}

BitBoard toBB(Ranks r) {
	return BB_RANK1 << (r << 3);
}

BitBoard toBB(Files f) {
	return BB_FILEA << f;
}

BitBoard shift(BitBoard b, Directions d) {
	return d > 0 ? b << d : b >> -d;
}