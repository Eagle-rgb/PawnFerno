#pragma once
#include "types.h"

constexpr BitBoard BB_EMPTY = 0;
constexpr BitBoard BB_FULL = ~BB_EMPTY;

constexpr BitBoard BB_a1 = 1;

constexpr BitBoard BB_RANK1 = 0xFF;
constexpr BitBoard BB_RANK2 = toBB(RANK2);
constexpr BitBoard BB_RANK3 = toBB(RANK3);
constexpr BitBoard BB_RANK4 = toBB(RANK4);
constexpr BitBoard BB_RANK5 = toBB(RANK5);
constexpr BitBoard BB_RANK6 = toBB(RANK6);
constexpr BitBoard BB_RANK7 = toBB(RANK7);
constexpr BitBoard BB_RANK8 = toBB(RANK8);

constexpr BitBoard BB_FILEA = 0x0101010101010101;
constexpr BitBoard BB_FILEB = toBB(FILEB);
constexpr BitBoard BB_FILEC = toBB(FILEC);
constexpr BitBoard BB_FILED = toBB(FILED);
constexpr BitBoard BB_FILEE = toBB(FILEE);
constexpr BitBoard BB_FILEF = toBB(FILEF);
constexpr BitBoard BB_FILEG = toBB(FILEG);
constexpr BitBoard BB_FILEH = toBB(FILEH);

/*const BitBoard RANKS_BB[8] = { 
	0x00000000000000FF, 
	0x000000000000FF00,
	0x0000000000FF0000,
	0x00000000FF000000,
	0x000000FF00000000,
	0x0000FF0000000000,
	0x00FF000000000000,
	0xFF00000000000000 };

const BitBoard FILES_BB[8] = {
	0x0101010101010101,
	0x0202020202020202,
	0x0404040404040404,
	0x0808080808080808,
	0x1010101010101010,
	0x2020202020202020,
	0x4040404040404040,
	0x8080808080808080 };*/

constexpr BitBoard toBB(Squares);
constexpr BitBoard toBB(Ranks);
constexpr BitBoard toBB(Files);

constexpr BitBoard shift(BitBoard, Directions);

BitBoard operator& (Squares sq, BitBoard b) {
	return toBB(sq) & b;
}

inline BitBoard operator<<(BitBoard b, Directions d) {

}