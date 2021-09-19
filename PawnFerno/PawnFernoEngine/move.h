#pragma once

#ifndef MOVE_INCLUDED
#define MOVE_INCLUDED

#include "types.h"
#include "bitboard.h"
#include <vector>

namespace move {
	constexpr short originSquareBits = 0b11111;

	/// <summary>
	/// Constructs a move using an origin and a destination square.
	/// </summary>
	constexpr Move makeMove(Square origin, Square destination) {
		return (short)origin | (short)(destination << 5);
	}

	/// <summary>
	/// Returns the origin square (first 5 bits) of the given move.
	/// </summary>
	constexpr Square originSquare(Move m) {
		return Square(m & originSquareBits);
	}

	/// <summary>
	/// Returns the destination square (bits 6 - 10) of the given move.
	/// </summary>
	constexpr Square destinationSquare(Move m) {
		return originSquare(m >> 5);
	}

	std::vector<Move> makeNormalMoves(Square from, BitBoard destinations);
}

#endif