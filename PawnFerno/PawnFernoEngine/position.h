#pragma once

#ifndef POSITION_INCLUDE
#define POSITION_INCLUDE

#include "bitboard.h"
#include "ioutil.h"
#include "fen.h"
#include "move.h"
#include <string>

class Position {
public:
	BitBoard BB_wb[2];
	BitBoard BB_pieces[6];
	Color player;
	int castlingRights;
	Square enPassant;

public:
	Position();
	Position(std::string fen);

	void clear();

	// Returns the piece standing on the given square for the current player.
	PieceType getPieceOn(Square);

	// Returns the piece standing on the given square for the given player.
	PieceType getPieceOn(Square, Color);

	// Returns the piece standing on the given square, for any player.
	// Also gives the player owning that piece as a reference parameter.
	// Gives black as color if the square is empty.
	PieceType getPieceOnAny(Square, Color&);

	void makeMove(const Move);

	std::string charBB();
	std::string print();
};

#endif

