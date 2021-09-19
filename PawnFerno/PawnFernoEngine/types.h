#pragma once

#ifndef TYPES_INCLUDE
#define TYPES_INCLUDE

#include <cstdint>

// Direction + 9 and then lookup in this array yields the appropriate direction index.
// -1 means no direction reaches this value.
constexpr int DIRECTION_INDEXES[19] = { 3, 2, 1, -1, -1, -1, -1, -1, 4, -1, 0, -1, -1, -1, -1, -1, 5, 6, 7 };
constexpr char RANK_CHARS[16] = { '1', '2', '3', '4', '5', '6', '7', '8', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
constexpr char FILE_CHARS[16] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
constexpr char PIECE_CHARS[8] = { 'p', 'n', 'b', 'r', 'q', 'k', ' ', ' ' };

typedef uint64_t BitBoard;

// First 5 bits are the origin square, next 5 bits are the destination square.
typedef short Move;

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

enum Color : bool {
	WHITE = false,
	BLACK = true
};

enum class Castling {
	None = 0,
	K = 0b1,
	Q = 0b10,
	k = 0b100,
	q = 0b1000
};

/// <summary>
/// Constructs the square residing on file f and rank r.
/// </summary>
constexpr Square toSquare(File f, Rank r) {
	return Square((8 * r) + f);
}

// Get Rank, File from Square
constexpr Rank toRank(Square sq) {
	return Rank(sq / 8);
}

constexpr File toFile(Square sq) {
	return File(int(sq) & 7);
}

/// <summary>
/// Returns the relative direction of square b to origin a.
/// </summary>
constexpr Direction relativeDirection(Square a, Square b) {
	int x = toFile(b) - toFile(a) > 0 ? 1 : (toFile(b) - toFile(a) < 0 ? -1 : 0);
	int y = toRank(b) - toRank(a) > 0 ? 8 : (toRank(b) - toRank(a) < 0 ? -8 : 0);
	return Direction(x + y);
}

/// <summary>
/// Gives the index starting from east going clockwise of the given direction.
/// </summary>
constexpr int directionIndex(Direction d) {
	return DIRECTION_INDEXES[d + 9];
}

/// <summary>
/// Converts a given character into the appropriate piece type. Case-Insensitive.
/// Following conversions are performed:
///		p : Pawn
///		n : Knight
///		b : Bishop
///		r : Rook
///		q : Queen
///		k : King
/// 
/// If the character does not match any of the above, PIECENONE will be returned.
/// </summary>
constexpr PieceType toPieceType(char pt) {
	// Makes pt lowercase.
	pt = pt < 'a' ? pt - 'A' + 'a' : pt;

	switch (pt) {
	case 'k': return KING;
	case 'q': return QUEEN;
	case 'r': return ROOK;
	case 'p': return PAWN;
	case 'b': return BISHOP;
	case 'n': return KNIGHT;
	default: return PIECENONE;
	}
}

/// <summary>
/// Returns the appropriate piece character for the given piece.
/// This also considers the player to move, so if black move, the character will be uppercase.
/// </summary>
constexpr char toChar(PieceType pt, Color who) {
	return pt == PIECENONE ? ' ' : 32 * (int)who + PIECE_CHARS[pt] - 32;
}

/// <summary>
/// Returns a character representation for a given rank (RANK1 -> 1)
/// </summary>
constexpr char rankChar(Rank r) {
	return RANK_CHARS[(int)r];
}

/// <summary>
/// Returns a character representation for a given file (FILEA -> a)
/// </summary>
constexpr char fileChar(File f) {
	return FILE_CHARS[(int)f];
}

/// <summary>
/// Basically casts the character to Castling enum. Returns None as default.
/// </summary>
constexpr Castling toCastlingValue(char c) {
	switch (c)
	{
	case 'K': return Castling::K;
	case 'k': return Castling::k;
	case 'Q': return Castling::q;
	case 'q': return Castling::Q;
	default: return Castling::None;
	}
}

#define ENABLE_ADD_OPERATORS_ON(T)												\
inline T operator+(T d1, int d2) { return T(int(d1) + d2); }					\
inline T& operator+=(T& d1, int d2) { return d1 = d1 + d2; }

#define ENABLE_BASE_OPERATORS_ON(T)												\
ENABLE_ADD_OPERATORS_ON(T)														\
inline T operator-(T d1, int d2) { return T(int(d1) - d2); }					\
inline T operator-(T d) { return T(-int(d)); }									\
inline T& operator-=(T& d1, int d2) { return d1 = d1 - d2; }

#define ENABLE_INCR_OPERATORS_ON(T)												\
inline T& operator++(T& d) { return d = T(int(d) + 1); }						\
inline T& operator--(T& d) { return d = T(int(d) - 1); }

#define ENABLE_LOGIC_OPERATORS_ON(T)											\
inline T operator!(T d) { return T(!(bool)d); }

ENABLE_BASE_OPERATORS_ON(Direction)

ENABLE_ADD_OPERATORS_ON(Square)
ENABLE_ADD_OPERATORS_ON(Castling)

ENABLE_INCR_OPERATORS_ON(PieceType)
ENABLE_INCR_OPERATORS_ON(Square)
ENABLE_INCR_OPERATORS_ON(File)
ENABLE_INCR_OPERATORS_ON(Rank)

ENABLE_LOGIC_OPERATORS_ON(Color)

#undef ENABLE_INCR_OPERATORS_ON
#undef ENABLE_BASE_OPERATORS_ON
#undef ENABLE_ADD_OPERATORS_ON

#endif