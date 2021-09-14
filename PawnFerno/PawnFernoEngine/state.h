#pragma once

#ifndef STATE_INCLUDED
#define STATE_INCLUDED

#include "types.h"

struct State {
	PieceType capturedPiece;
	int castlingRights;
	Square enPassant;

	State* previousState;
};

constexpr State startState() {
	State s = State();

	s.capturedPiece = PIECENONE;
	s.castlingRights = 15;
	s.enPassant = SQNONE;

	s.previousState = nullptr;

	return s;
}

#endif