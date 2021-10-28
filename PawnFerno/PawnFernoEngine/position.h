#pragma once

#ifndef POSITION_INCLUDE
#define POSITION_INCLUDE

#include "bitboard.h"
#include "ioutil.h"
#include "fen.h"
#include "move.h"
#include "evaluation.h"
#include "state.h"
#include <vector>
#include <string>
#include <chrono>

constexpr char startPosFen[57] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

class Position {
private:
	BitBoard BB_wb[2];
	BitBoard BB_pieces[6];
	Square pieceSquares[12][16];
	char pieceCounts[12];  // char just so it is only one byte.

	Color player;
	State* state;

    char findPieceSquaresIndex(const Piece piece, const Square sq);

	/// <summary>
	/// Moves the given piece from the origin to destination. Does not perform any assertions!.
	/// Also, does not perform any captures.
	/// </summary>
	void movePiece(const Square origin, const Square destination, const PieceType, const Color);

	/// <summary>
	/// Removes ("Captures") the piece.
	/// </summary>
	void removePiece(const Square, const PieceType, const Color);

	/// <summary>
	/// Adds the given piece to the board.
	/// </summary>
	void addPiece(const Square, const PieceType, const Color);

	/// <summary>
	/// Calculates and returns all legal pawn moves.
	/// </summary>
	std::vector<Move> getLegalPawnMoves(const BitBoard& checkRay);  // Non const - as it tries for enPassant.

	/// <summary>
	/// Calculates and returns all legal slider moves.
	/// </summary>
	std::vector<Move> getLegalSliderMoves(const BitBoard& checkRay) const;

	std::vector<Move> getLegalCastlings() const;

	void updateRepetition();

	Score static_eval_for(Color) const;

public:
	Position(State*);
	Position(std::string fen, State*);
	Position(std::vector<std::string>& fenParts, State*);
	Position(Position&) = delete;  // We do not want a copy constructor for now. Best to disable it.

	/// <summary>
	/// Clears the current position and sets all values to default values.
	/// </summary>
	void clear();

	Color playerToMove() const;

	/// <summary>
	/// Returns the piece standing on the given square for the current player.
	/// </summary>
	PieceType getPieceOn(const Square) const;

	/// <summary>
	/// Returns the piece standing on the given square for the given player.
	/// </summary>
	PieceType getPieceOn(const Square, const Color) const;

	/// <summary>
	/// Returns the piece standing on the given square, for any player.
	/// Also gives the player owning that piece as a reference parameter.
	/// Gives black as color if the square is empty.
	/// </summary>
	PieceType getPieceOnAny(const Square, Color&) const;

	/// <summary>
	/// Returns the blocker bitboard.
	/// </summary>
	inline BitBoard blockerBB() const { return BB_wb[WHITE] | BB_wb[BLACK]; }

	/// <summary>
	/// Returns the piece BitBoard of the given piece for the current player.
	/// </summary>
	inline BitBoard playerPieceBBof(const PieceType p) const { return BB_wb[player] & BB_pieces[p]; }

	/// <summary>
	/// Returns the square of the king for the current player.
	/// </summary>
	inline Square kingSquare() const { return toSquare(playerPieceBBof(KING)); }

	/// <summary>
	/// Determines whether a given move performs a capture.
	/// </summary>
	bool isCaptures(const Move&) const;

	/// <summary>
	/// Returns true if the current player is in check.
	/// PREREQUISITE: 0b1
	/// </summary>
	bool inCheck() const;

	/// <summary>
	/// Returns true if the current player is in double check.
	/// PREREQUISITE: 0b1
	/// </summary>
	bool inDoubleCheck() const;

	/// <summary>
	/// Returns true if the current player is mated.
	/// PREREQUISITE: 0b1001
	/// </summary>
	bool isMate() const;

	/// <summary>
	/// Returns true if the current position is a draw.
	/// PREREQUISITE: 0b1001
	/// </summary>
	bool isDraw() const;

	/// <summary>
	/// Returns true if the piece on the given square is pinned.
	/// PREREQUISITE: 0b100
	/// </summary>
	bool isPinned(const Square) const;

	/// <summary>
	/// Calculates all squares being attacked by the given player and return them.
	/// Optionally, one may exclude the enemies king from the board.
	/// </summary>
	/// <returns> BitBoard containing the attacked squares. </returns>
	BitBoard getAttacksOf(const Color who, bool excludeKing = false) const;

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
	/// Calculates all legal moves and does all the prerequisites automatically :).
	/// </summary>
	std::vector<Move> getLegalMovesAuto();

	/// <summary>
	/// Tries the given move and return true if the resulting position remains legal.
	/// </summary>
	bool tryMove(const Move&);

	/// <summary>
	/// Makes the given move on the board.
	/// </summary>
	void makeMove(const Move&, State& newState);

	/// <summary>
	/// Makes the move, but forgets how to undo it (reuses its own state object).
	/// </summary>
	void makeMoveForgetful(const Move&);

	/// <summary>
	/// Undo's the given move.
	/// </summary>
	void undoMove(const Move&);

	/// <summary>
	/// This takes a pseudo move - mostlikely generated from algebraic notation - and converts it into a move fitting the current position.
	/// </summary>
	Move makeLegalFromPseudo(const Move);

	// EVALUATION TOOLS
	/// <summary>
	/// Statically evaluates this position.
	/// PREREQUISITES: 0b1001
	/// </summary>
	Score static_eval() const;

	Score eval_move(const Move& m) const;

	void sortMoves(MoveList& moves) const;

	unsigned int getRule50Count() const;

	unsigned int getPlyCount() const;

	unsigned int getFullMoveCount() const;

	std::string toFen() const;

	/// <summary>
	/// Returns a character-string like in FEN but without the '/' and with spaces for no pieces.
	/// </summary>
	std::string charBB() const;

	/// <summary>
	/// Returns a printable string of the given position.
	/// </summary>
	std::string print() const;
};

#endif

