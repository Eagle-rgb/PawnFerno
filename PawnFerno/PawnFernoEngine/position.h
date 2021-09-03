#pragma once

#ifndef POSITION_INCLUDE
#define POSITION_INCLUDE

#include "bitboard.h"
class Position {
private:
	BitBoard wbBB[2];
	BitBoard piecesBB[6];
	int castlingRights;
	Square enPassant;

public:

};

#endif

