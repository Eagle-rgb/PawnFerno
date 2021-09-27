#pragma once

#ifndef TYPES_INCLUDE
#define TYPES_INCLUDE

#include <cstdint>
#include <cstdlib>
#include <string>

// Direction + 9 and then lookup in this array yields the appropriate direction index.
// -1 means no direction reaches this value.
constexpr int DIRECTION_INDEXES[19] = { 3, 2, 1, -1, -1, -1, -1, -1, 4, -1, 0, -1, -1, -1, -1, -1, 5, 6, 7 };
constexpr char PIECE_CHARS[13] = { 'P', 'N', 'B', 'R', 'Q', 'K', 'p', 'n', 'b', 'r', 'q', 'k', ' ' };

typedef uint64_t BitBoard;

// First 5 bits are the origin square, next 5 bits are the destination square.
typedef unsigned short Move;

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

enum Piece {
	W_PAWN = 0, W_KNIGHT = 1, W_BISHOP = 2, W_ROOK = 3, W_QUEEN = 4, W_KING = 5,
	B_PAWN = 6, B_KNIGHT = 7, B_BISHOP = 8, B_ROOK = 9, B_QUEEN = 10, B_KING = 11,
	WB_PIECENONE = 12
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

enum class SCastling {
	None = 0,
	K = 0b01,
	Q = 0b10
};

/// <summary>
/// Constructs the square residing on file f and rank r.
/// </summary>
constexpr Square toSquare(const File f, const Rank r) {
	return Square((8 * r) + f);
}

// Get Rank, File from Square
constexpr Rank toRank(const Square sq) {
	return Rank(sq / 8);
}

constexpr File toFile(const Square sq) {
	return File(int(sq) & 7);
}

/// <summary>
/// Gives the index starting from east going clockwise of the given direction.
/// </summary>
constexpr int directionIndex(const Direction d) {
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

constexpr Piece makeColoredPiece(const PieceType pt, const Color who) {
	return pt == PIECENONE ? WB_PIECENONE : Piece(pt + 6 * who);
}

/// <summary>
/// Returns the appropriate piece character for the given piece.
/// This also considers the player to move, so if black move, the character will be uppercase.
/// </summary>
inline char toChar(const PieceType pt, const Color who) {
	return PIECE_CHARS[makeColoredPiece(pt, who)];
}

constexpr PieceType toPieceType(const Piece piece) {
	return piece < 6 ? PieceType(piece) : PieceType(piece - 6);
}

constexpr bool isOfColor(const Piece piece, const Color who) {
	return (piece < 6) ^ who;
}

/// <summary>
/// Returns a character representation for a given rank (RANK1 -> 1)
/// </summary>
constexpr char rankChar(const Rank r) {
	return '1' + r;
}

/// <summary>
/// Returns a character representation for a given file (FILEA -> a)
/// </summary>
constexpr char fileChar(const File f) {
	return 'a' + f;
}

/// <summary>
/// Basically casts the character to Castling enum. Returns None as default.
/// </summary>
constexpr Castling toCastlingValue(const char c) {
	switch (c)
	{
	case 'K': return Castling::K;
	case 'k': return Castling::k;
	case 'Q': return Castling::Q;
	case 'q': return Castling::q;
	default: return Castling::None;
	}
}

constexpr Castling toCastling(SCastling sc, Color who) {
	return Castling((short)sc << who);
}

/// <summary>
/// Maps any type of castling to an appropriate index for lookups in castling arrays (defined in bitboard.h)
/// <summary>
constexpr int castlingIndex(const Color who, const SCastling c) {
	return ((short)c + 2 * (short)who) - 1;
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
ENABLE_INCR_OPERATORS_ON(Piece)
ENABLE_INCR_OPERATORS_ON(Square)
ENABLE_INCR_OPERATORS_ON(File)
ENABLE_INCR_OPERATORS_ON(Rank)

ENABLE_LOGIC_OPERATORS_ON(Color)

#undef ENABLE_INCR_OPERATORS_ON
#undef ENABLE_BASE_OPERATORS_ON
#undef ENABLE_ADD_OPERATORS_ON

#endif