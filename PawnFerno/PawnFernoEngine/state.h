#pragma once

#ifndef STATE_INCLUDED
#define STATE_INCLUDED

#include "types.h"

struct State {
	PieceType capturedPiece;
	int castlingRights;
	Color player;
	Square enPassant;
};

#endif