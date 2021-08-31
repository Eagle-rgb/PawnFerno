#pragma once
#include <cstdint>

#ifndef TYPES_INCLUDE
#define TYPES_INCLUDE


typedef uint64_t BitBoard;

enum Squares {
	SQa1, SQb1, SQc1, SQd1, SQe1, SQf1, SQg1, SQh1,
	SQa2, SQb2, SQc2, SQd2, SQe2, SQf2, SQg2, SQh2,
	SQa3, SQb3, SQc3, SQd3, SQe3, SQf3, SQg3, SQh3,
	SQa4, SQb4, SQc4, SQd4, SQe4, SQf4, SQg4, SQh4,
	SQa5, SQb5, SQc5, SQd5, SQe5, SQf5, SQg5, SQh5,
	SQa6, SQb6, SQc6, SQd6, SQe6, SQf6, SQg6, SQh6,
	SQa7, SQb7, SQc7, SQd7, SQe7, SQf7, SQg7, SQh7,
	SQa8, SQb8, SQc8, SQd8, SQe8, SQf8, SQg8, SQh8
};

inline Squares operator++ (Squares sq) {
	return Squares(sq + 1);
}
inline Squares operator++(Squares& sq, int)
{
  Squares tmp(sq);
  ++sq;
  return tmp;
}

inline Squares operator-- (Squares sq) {
	return Squares(sq - 1);
}

enum Ranks {
	RANK1, RANK2, RANK3, RANK4, RANK5, RANK6, RANK7, RANK8
};

enum Files {
	FILEA, FILEB, FILEC, FILED, FILEE, FILEF, FILEG, FILEH
};

enum PieceTypes {
	PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING
};

enum Directions {
	NORTH = 8,
	EAST = 1,
	SOUTH = -NORTH,
	WEST = -EAST,

	SOUTHEAST = SOUTH + EAST,
	SOUTHWEST = SOUTH + WEST,
	NORTHWEST = NORTH + WEST,
	NORTHEAST = NORTH + EAST
};

enum Colors {
	WHITE = 0, BLACK = 1
};

#endif