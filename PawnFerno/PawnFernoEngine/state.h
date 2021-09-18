#pragma once

#ifndef STATE_INCLUDED
#define STATE_INCLUDED

#include "types.h"
#include "bitboard.h"
#include <vector>

constexpr short generatedEnemyAttacks = 0b1;

struct State {
	PieceType capturedPiece;
	short castlingRights;
	Square enPassant;

	// Move generation
	BitBoard enemyAttacks;
	std::vector<Move> legalMoves;

	// Move generation check (see constexpr above for bit sets)
	short moveGenCheck;

	State* previousState;

	State();
};

#endif