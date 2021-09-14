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

	/// <summary>
	/// Clears the current position and sets all values to default values.
	/// </summary>
	void clear();

	/// <summary>
	/// Returns the piece standing on the given square for the current player.
	/// </summary>
	PieceType getPieceOn(Square);

	/// <summary>
	/// Returns the piece standing on the given square for the given player.
	/// </summary>
	PieceType getPieceOn(Square, Color);

	/// <summary>
	/// Returns the piece standing on the given square, for any player.
	/// Also gives the player owning that piece as a reference parameter.
	/// Gives black as color if the square is empty.
	/// </summary>
	PieceType getPieceOnAny(Square, Color&);

	/// <summary>
	/// Makes the given move on the board.
	/// </summary>
	void makeMove(const Move);

	/// <summary>
	/// Returns a character-string like in FEN but without the '/' and with spaces for no pieces.
	/// </summary>
	std::string charBB();

	/// <summary>
	/// Returns a printable string of the given position.
	/// </summary>
	std::string print();
};

#endif

