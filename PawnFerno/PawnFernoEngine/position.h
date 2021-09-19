#pragma once

#ifndef POSITION_INCLUDE
#define POSITION_INCLUDE

#include "bitboard.h"
#include "ioutil.h"
#include "fen.h"
#include "move.h"
#include "state.h"
#include <vector>
#include <string>

constexpr char startPosFen[57] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

class Position {
private:
	BitBoard BB_wb[2];
	BitBoard BB_pieces[6];

	Color player;

	/// <summary>
	/// Moves the given piece from the origin to destination. Does not perform any assertions!.
	/// Also, does not perform any captures.
	/// </summary>
	void movePiece(Square origin, Square destination, PieceType, Color);

public:
	State* state;

	Position(State*);
	Position(std::string fen, State*);

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
	/// Calculates all squares being attacked by the enemy and returns them.
	/// </summary>
	/// <returns> BitBoard containing the attacked squares. </returns>
	BitBoard getEnemyAttacks();

	/// <summary>
	/// Calculates all pins and checkers on the current player. 
	/// Saves pinned pieces and checkers in the state.
	/// </summary>
	void makePinnersAndCheckers();

	/// <summary>
	/// Calculates all legal moves.
	/// PREREQUISITE: 0b111
	/// </summary>
	std::vector<Move> getLegalMoves();

	/// <summary>
	/// Returns true if the current player is in check.
	/// PREREQUISITE: 0b1
	/// </summary>
	bool inCheck();

	/// <summary>
	/// Returns true if the current player is in double check.
	/// </summary>
	bool inDoubleCheck();

	/// <summary>
	/// Returns true if the piece on the given square is pinned.
	/// PREREQUISITE: 0b100
	/// </summary>
	bool isPinned(Square);

	/// <summary>
	/// Makes the given move on the board.
	/// </summary>
	void makeMove(const Move, State& newState);

	/// <summary>
	/// Undo's the given move.
	/// </summary>
	void undoMove(const Move);

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

