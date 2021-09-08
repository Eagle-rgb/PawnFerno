#pragma once

#ifndef POSITION_INCLUDE
#define POSITION_INCLUDE

#include "bitboard.h"
#include "fen.h"
#include <string>
class Position {
private:
	BitBoard BB_wb[2];
	BitBoard BB_pieces[6];
	int castlingRights;
	Square enPassant;

public:
	Position();
	Position(std::string fen);

};

#endif

