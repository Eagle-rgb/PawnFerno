#include "bitboard.h"
#include <iostream>

BitBoard PAWN_PUSHES[2][64];
BitBoard PAWN_CAPTURES[2][64];
BitBoard KNIGHT_ATTACKS[64];
BitBoard KING_ATTACKS[64];

using namespace std;

void print(BitBoard);