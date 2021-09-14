#pragma once

#ifndef FEN_INCLUDED
#define FEN_INCLUDED

#include "misc.h"
#include "types.h"
#include <assert.h>

namespace fen {
	using namespace std;
	constexpr char pieceChars[12] = { 'K', 'Q', 'R', 'B', 'N', 'P', 'k', 'q', 'r', 'b', 'n', 'p' };
	constexpr char castlingChars[4] = { 'k', 'K', 'q', 'Q' };

	string toCharBB(vector<string>);
	Color sideToMove(string);
	Castling castlingRights(string);
	Square enPassant(string);
	int halfClock(string);
	int fullClock(string);
}
#endif