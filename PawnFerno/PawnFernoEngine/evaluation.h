#pragma once

#ifndef EVALUATION_INCLUDED
#define EVALUATION_INCLUDED

#include "types.h"
#include "bitboard.h"

namespace eval {
	constexpr Score PIECE_VALUES[6] = { 100, 300, 350, 500, 900, 100000 };
	constexpr Score SQVALUES_PAWN[64] = {
		0,  0,  0,  0,  0,  0,  0,  0,
		5, 10,  5,-30,-30, 10,  5,  5,
		5, -5, -5,  0,  0, -5, -5,  5,
		0,  0,  0, 20, 20,  0,  0,  0,
		5,  5, 10, 25, 25, 10,  5,  5,
		10, 10, 20, 30, 30, 20, 10, 10,
		70, 70, 70, 70, 70, 70, 70, 70,
		0,  0,  0,  0,  0,  0,  0,  0
	};

	constexpr Score SQVALUES_KNIGHT[64] = {
		-40,-25,-35,-30,-30,-35,-25,-40,
		-30,-20,  0,  5,  5,  0,-20,-30,
		-20,  5, 10, 15, 15, 10,  5,-20,
		-20,  0, 15, 20, 20, 15,  0,-20,
		-20,  5, 15, 20, 20, 15,  5,-20,
		-20,  0, 10, 15, 15, 10,  0,-20,
		-30,-20,  0,  0,  0,  0,-20,-30,
		-40,-35,-30,-30,-30,-30,-35,-40
	};

	constexpr Score SQVALUES_BISHOP[64] = {
		-20,-10,-10,-10,-10,-10,-10,-20,
		-10,  5,  0,  0,  0,  0,  5,-10,
		-10, 10, 10, 10, 10, 10, 10,-10,
		-10,  0, 10, 10, 10, 10,  0,-10,
		-10,  5,  5, 10, 10,  5,  5,-10,
		-10,  0,  5, 10, 10,  5,  0,-10,
		-10,  0,  0,  0,  0,  0,  0,-10,
		-20,-10,-10,-10,-10,-10,-10,-20
	};

	constexpr Score SQVALUES_ROOK[64] = {
		 0,  0,  0,  5,  5,  0,  0,  0,
		-5,  0,  0,  0,  0,  0,  0, -5,
		-5,  0,  0,  0,  0,  0,  0, -5,
		-5,  0,  0,  0,  0,  0,  0, -5,
		-5,  0,  0,  0,  0,  0,  0, -5,
		-5,  0,  0,  0,  0,  0,  0, -5,
		-5, 10, 10, 10, 10, 10, 10,  5,
		0,  0,  0,  0,  0,  0,  0,  0
	};

	constexpr Score SQVALUES_QUEEN[64] = {
		-20,-10,-10, -5, -5,-10,-10,-20,
		-10,  0,  5,  0,  0,  0,  0,-10,
		-10,  5,  5,  5,  5,  5,  0,-10,
		0,  0,  5,  5,  5,  5,  0, -5,
		-5,  0,  5,  5,  5,  5,  0, -5,
		-10,  0,  5,  5,  5,  5,  0,-10,
		-10,  0,  0,  0,  0,  0,  0,-10,
		-20,-10,-10, -5, -5,-10,-10,-20
	};

	constexpr Score SQVALUES_KING[64] = {
		25, 30, 20,  0,  0, 20, 30, 25,
		20, 20,-10,-20,-20,-10, 20, 20,
		-10,-20,-20,-20,-20,-20,-20,-10,
		-20,-30,-30,-40,-40,-30,-30,-20,
		-30,-40,-40,-50,-50,-40,-40,-30,
		-30,-40,-40,-50,-50,-40,-40,-30,
		-30,-40,-40,-50,-50,-40,-40,-30,
		-30,-40,-40,-50,-50,-40,-40,-30
	};

	extern Score SQUARE_VALUES[2][6][64];

	enum Scores : int {
		VALUE_MATE = -1000000,
		VALUE_DRAW = 0,

		VALUE_PAWN = 10,
		VALUE_KNIGHT = 30,
		VALUE_BISHOP = 35,
		VALUE_ROOK = 50,
		VALUE_QUEEN = 90
	};

	void init();

	constexpr Score getPieceScore(const PieceType pt) { return PIECE_VALUES[pt]; }
	constexpr Score getPieceSquareScore(const PieceType pt, const Square sq, const Color who) { return SQUARE_VALUES[who][pt][sq]; }
}
#endif