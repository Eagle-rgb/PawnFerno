#include "state.h"

State::State() :
	capturedPiece(PIECENONE),
	castlingRights(0),
	enPassant(SQNONE),

	enemyAttacks(BB_EMPTY),
	legalMoves(std::vector<Move>()),

	previousState(nullptr) { };