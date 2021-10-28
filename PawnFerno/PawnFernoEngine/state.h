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
	// Important for position equality.
	short castlingRights;
	Square enPassant;

	// -------------------------------
	// Not important for position equality.

	PieceType capturedPiece;

	// Move generation
	BitBoard enemyAttacks;
	BitBoard pinnedPieces;
	BitBoard checkingPieces;
	std::vector<Move> legalMoves;

	// Move generation check (see constexpr above for bit sets)
	short moveGenCheck;

	// Counters
	unsigned int rule50;
	unsigned int ply;

	unsigned short repetition;

	State* previousState;

	State() {
		capturedPiece = PIECENONE,
		castlingRights = 0,
		enPassant = SQNONE,
		moveGenCheck = 0,
		previousState = nullptr,
		rule50 = 0,
		ply = 0;
	}

	inline bool canCastleSide(SCastling c, Color who) { return (short)toCastling(c, who) & castlingRights; }
};

#endif