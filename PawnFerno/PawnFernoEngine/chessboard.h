#pragma once
#include "bitboard.h"

class Chessboard {
private:
	BitBoard BB_wb[2];
	BitBoard BB_pieces[6];

public:
	Chessboard();
};
