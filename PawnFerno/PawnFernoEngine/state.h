#pragma once

#ifndef STATE_INCLUDED
#define STATE_INCLUDED

#include "types.h"
#include "bitboard.h"
#include <vector>

constexpr short generatedEnemyAttacks = 0b1;
constexpr short generatedCheckingPieces = 0b10;
constexpr short generatedPinnedPieces = 0b100;
constexpr short generatedLegalMoves = 0b1000;

struct State {
	PieceType capturedPiece;
	short castlingRights;
	Square enPassant;

	// Move generation
	BitBoard enemyAttacks;
	BitBoard pinnedPieces;
	BitBoard checkingPieces;
	std::vector<Move> legalMoves;

	// Move generation check (see constexpr above for bit sets)
	short moveGenCheck;

	State* previousState;

	State();
};

#endif