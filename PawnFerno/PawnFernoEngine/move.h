#pragma once

#ifndef MOVE_INCLUDED
#define MOVE_INCLUDED

#include "types.h"
#include "bitboard.h"
#include <vector>

namespace move {
	enum SpecialMoves {
		enPassant = 0b1 << 10,
		castling = 0b10 << 10,
		promotion = 0b100
	};

	constexpr short originSquareBits = 0b11111;

	/// <summary>
	/// Constructs a move using an origin and a destination square.
	/// No en passant
	/// No castling
	/// No promotion
	/// </summary>
	constexpr Move makeMove(Square origin, Square destination) {
		return (short)origin | (short)(destination << 5);
	}

	constexpr Move makeEnPassantMove(Square origin, Square destination) {
		return (short)origin | (short)(destination << 5) | enPassant;
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

	constexpr bool isEnPassant(Move m) { return (short)m & enPassant; }

	std::vector<Move> makeNormalMoves(Square from, BitBoard destinations);
}

#endif