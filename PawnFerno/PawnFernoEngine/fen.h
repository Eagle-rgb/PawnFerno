#pragma once

#ifndef FEN_INCLUDED
#define FEN_INCLUDED

#include "misc.h"
#include "types.h"
#include <assert.h>

using namespace std;

namespace fen {
	constexpr char pieceChars[12] = { 'K', 'Q', 'R', 'B', 'N', 'P', 'k', 'q', 'r', 'b', 'n', 'p' };
	constexpr char castlingChars[4] = { 'k', 'K', 'q', 'Q' };

	string toCharBB(vector<string>);
	Color sideToMove(string);
	castling::Castling castlingRights(string);
	Square enPassant(string);
	int halfClock(string);
	int fullClock(string);
}
#endif