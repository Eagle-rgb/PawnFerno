#pragma once
#ifndef TYPES_INCLUDE
#define TYPES_INCLUDE

#include <cstdint>

// Direction + 9 and then lookup in this array yields the appropriate direction index.
// -1 means no direction reaches this value.
constexpr int DIRECTION_INDEXES[19] = { 3, 2, 1, -1, -1, -1, -1, -1, 4, -1, 0, -1, -1, -1, -1, -1, 5, 6, 7 };

typedef uint64_t BitBoard;

enum Square {
	SQa1, SQb1, SQc1, SQd1, SQe1, SQf1, SQg1, SQh1,
	SQa2, SQb2, SQc2, SQd2, SQe2, SQf2, SQg2, SQh2,
	SQa3, SQb3, SQc3, SQd3, SQe3, SQf3, SQg3, SQh3,
	SQa4, SQb4, SQc4, SQd4, SQe4, SQf4, SQg4, SQh4,
	SQa5, SQb5, SQc5, SQd5, SQe5, SQf5, SQg5, SQh5,
	SQa6, SQb6, SQc6, SQd6, SQe6, SQf6, SQg6, SQh6,
	SQa7, SQb7, SQc7, SQd7, SQe7, SQf7, SQg7, SQh7,
	SQa8, SQb8, SQc8, SQd8, SQe8, SQf8, SQg8, SQh8,
	SQNONE = 64
};

enum Rank {
	RANK1, RANK2, RANK3, RANK4, RANK5, RANK6, RANK7, RANK8, RANKNONE = 15
};

enum File {
	FILEA, FILEB, FILEC, FILED, FILEE, FILEF, FILEG, FILEH, FILENONE = 15
};

enum PieceType {
	PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, PIECENONE = 7
};

enum Direction {
	NORTH = 8,
	EAST = 1,
	SOUTH = -NORTH,
	WEST = -EAST,

	SOUTHEAST = SOUTH + EAST,
	SOUTHWEST = SOUTH + WEST,
	NORTHWEST = NORTH + WEST,
	NORTHEAST = NORTH + EAST,

	DIRECTIONNONE = 0
};

enum Color {
	WHITE = 0,
	BLACK = 1
};

int directionIndex(Direction);

#define ENABLE_ADD_OPERATORS_ON(T)												\
constexpr T operator+(T d1, int d2) { return T(int(d1) + d2); }					\
inline T& operator+=(T& d1, int d2) { return d1 = d1 + d2; }

#define ENABLE_BASE_OPERATORS_ON(T)												\
ENABLE_ADD_OPERATORS_ON(T)														\
constexpr T operator-(T d1, int d2) { return T(int(d1) - d2); }					\
constexpr T operator-(T d) { return T(-int(d)); }								\
inline T& operator-=(T& d1, int d2) { return d1 = d1 - d2; }

#define ENABLE_INCR_OPERATORS_ON(T)												\
inline T& operator++(T& d) { return d = T(int(d) + 1); }						\
inline T& operator--(T& d) { return d = T(int(d) - 1); }						

ENABLE_BASE_OPERATORS_ON(Direction)

ENABLE_ADD_OPERATORS_ON(Square)

ENABLE_INCR_OPERATORS_ON(PieceType)
ENABLE_INCR_OPERATORS_ON(Square)
ENABLE_INCR_OPERATORS_ON(File)
ENABLE_INCR_OPERATORS_ON(Rank)
ENABLE_INCR_OPERATORS_ON(Color)

#undef ENABLE_INCR_OPERATORS_ON
#undef ENABLE_BASE_OPERATORS_ON
#undef ENABLE_ADD_OPERATORS_ON

#endif