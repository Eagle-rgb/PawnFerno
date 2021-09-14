#pragma once

#ifndef MOVE_INCLUDED
#define MOVE_INCLUDED

#include "types.h"

namespace move {
	constexpr short originSquareBits = 0b11111;

	constexpr Move makeMove(Square origin, Square destination) {
		return (short)origin | (short)(destination << 5);
	}

	constexpr Square originSquare(Move m) {
		return Square(m & originSquareBits);
	}

	constexpr Square destinationSquare(Move m) {
		return originSquare(m >> 5);
	}
}

#endif