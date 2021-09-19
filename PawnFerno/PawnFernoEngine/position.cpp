#include "position.h"

Position::Position(State* state) {
	BB_wb[WHITE] = BB_RANK1 | BB_RANK2;
	BB_wb[BLACK] = BB_RANK7 | BB_RANK8;

	BB_pieces[PAWN] = BB_RANK2 | BB_RANK7;
	BB_pieces[KNIGHT] = toBB(SQb1) | toBB(SQg1) | toBB(SQb8) | toBB(SQg8);
	BB_pieces[BISHOP] = toBB(SQc1) | toBB(SQc8) | toBB(SQf1) | toBB(SQf8);
	BB_pieces[ROOK] = toBB(SQa1) | toBB(SQa8) | toBB(SQh1) | toBB(SQh8);
	BB_pieces[QUEEN] = toBB(SQd1) | toBB(SQd8);
	BB_pieces[KING] = toBB(SQe1) | toBB(SQe8);

	player = WHITE;
	this->state = state;
}

Position::Position(std::string fen, State* state) {
	// Initialization, default values.
	clear();

	this->state = state;
	std::vector<std::string> fenParts = misc::split(fen, ' ');

	std::vector<std::string>::iterator it = fenParts.begin();

	char charBB[64];

	// Board
	fen::toCharBB(misc::split(*it, '/'), charBB);

	// Goes through every square, reads the appropriate entry in the charBB and updates
	// the appropriate piece bitboard as well as player bitboard.
	for (Rank i = RANK1; i <= RANK8; ++i) {
		for (File j = FILEA; j <= FILEH; ++j) {
			Square sq = toSquare(j, i);

			char c = charBB[sq];

			if (c == ' ') { ++j; continue; }

			BitBoard current = toBB(sq);
			if (misc::isUpper(c)) BB_wb[(int)WHITE] |= current;
			else BB_wb[(int)BLACK] |= current;

			PieceType pt = toPieceType(c);
			BB_pieces[pt] |= current;
		}
	}

	++it;

	// Player to move.
	player = fen::sideToMove(*it);
	++it;

	// Castling rights.
	state -> castlingRights = (short)fen::castlingRights(*it);
	++it;

	// En Passant square.
	state -> enPassant = fen::enPassant(*it);
	++it;


	// Half move clock. TODO

	// Full move clock. TODO
}

void Position::clear() {
	BB_wb[0] = BB_EMPTY; BB_wb[1] = BB_EMPTY;

	for (PieceType i = PAWN; i <= KING; ++i) {
		BB_pieces[i] = BB_EMPTY;
	}

	player = WHITE;
}

PieceType Position::getPieceOn(Square sq) {
	return getPieceOn(sq, player);
}

PieceType Position::getPieceOn(Square sq, Color who) {
	BitBoard currentPlayerBB = BB_wb[who];

	for (PieceType i = PAWN; i <= KING; ++i)
		if (!isFree(BB_pieces[i] & currentPlayerBB, sq)) return i;

	return PIECENONE;
}

PieceType Position::getPieceOnAny(Square sq, Color& c) {
	for (PieceType i = PAWN; i <= KING; ++i) {
		if (!isFree(BB_pieces[i], sq)) {
			c = !isFree(BB_wb[WHITE], sq) ? WHITE : BLACK;
			return i;
		}
	}

	return PIECENONE;
}

BitBoard Position::getEnemyAttacks() {
	BitBoard enemyPieces[6];
	BitBoard blockers = BB_wb[0] | BB_wb[1];

	for (PieceType p = PAWN; p <= KING; ++p) {
		enemyPieces[p] = BB_pieces[p] & BB_wb[!player];
	}

	state->moveGenCheck |= generatedEnemyAttacks;
	return (state->enemyAttacks = attacksOfAll(enemyPieces, blockers, !player));
}

BitBoard Position::getPinners() {
	Square kingSquare = toSquare(BB_wb[player] & BB_pieces[KING]);
	BitBoard pinnerRays = BB_EMPTY;
	BitBoard checkingPieces = BB_EMPTY;

	// Remember that only sliders can pin.

	for (PieceType p = BISHOP; p <= QUEEN; ++p) {
		BitBoard sliders = BB_wb[!player] & BB_pieces[p];
		Square sq = SQNONE;

		while ((sq = bits::popLSB(sliders)) != SQNONE) {
			Direction toKing = relativeDirection(sq, kingSquare);

			// Abort if this piece can not attack in that direction.
			if (p == BISHOP && (abs(toKing) == 1 || abs(toKing) == 8)) continue;
			else if (p == ROOK && (abs(toKing) == 7 || abs(toKing) == 9)) continue;

			// Shoot ray from the piece to the king and backwards and see if they meet.
			BitBoard shotToKing = RAYS[sq][toKing];
			BitBoard shotFromKing = RAYS[kingSquare][-toKing];

			if (isEmpty(shotToKing & shotFromKing)) continue;

			BitBoard between = shotToKing & shotFromKing & BB_wb[player];

			// No enemy piece may be in the line of sight of the pinner.
			if (!isEmpty(between & BB_wb[!player])) continue;

			// There can only be none or at most one piece in the line of sight.
			if (more_than_one(between)) continue;

			// If no piece is in the way this even is a checking piece (awesome :)).
			if (isEmpty(between)) checkingPieces |= toBB(sq);

			// Finally, this is a checking piece or a pinner (we will save the ray in both cases).
			// Doing it this way will include the pinner's square and the king square.
			pinnerRays |= sq | (shotToKing ^ RAYS[kingSquare][toKing]) | kingSquare;
		}

	}

	return 0;
}

bool Position::inCheck() {
	assert((state->moveGenCheck & generatedEnemyAttacks) != 0);
	return !isEmpty(BB_wb[player] & BB_pieces[KING] & state->enemyAttacks);
}

void Position::movePiece(Square origin, Square destination, PieceType piece, Color who) {
	BitBoard orDesBB = toBB(origin) | toBB(destination);

	BB_wb[who] ^= orDesBB;
	BB_pieces[piece] ^= orDesBB;
}

void Position::makeMove(const Move m, State& newState) {
	Square originSquare = move::originSquare(m);
	Square destinationSquare = move::destinationSquare(m);
	PieceType movedPiece = getPieceOn(originSquare);
	PieceType capturedPiece = getPieceOn(destinationSquare);

	newState.previousState = state;
	state = &newState;

	// TODO assert that there is actually a piece standing on originSquare and no piece on destinationSquare.
	movePiece(originSquare, destinationSquare, movedPiece, player);

	state -> capturedPiece = capturedPiece;
	player = !player;
}

void Position::undoMove(const Move m) {
	assert(state->previousState != nullptr);

	Square destinationSquare = move::destinationSquare(m);
	Square originSquare = move::originSquare(m);

	// The previous player moved the piece. Get that piece.
	PieceType movedPiece = getPieceOn(destinationSquare, !player);
	assert(movedPiece != PIECENONE);

	PieceType capturedPiece = state -> capturedPiece;

	Color playerWhoMoved = !player;

	// Move the piece from the destination square to the origin square.
	movePiece(destinationSquare, originSquare, movedPiece, playerWhoMoved);

	state = state->previousState;

	player = playerWhoMoved;
}

std::string Position::charBB() {
	std::string result = "";

	for (Rank i = RANK8; i >= RANK1; --i) {
		for (File j = FILEA; j <= FILEH; ++j) {
			Color who = WHITE;
			Square sq = toSquare(j, i);
			PieceType pieceOn = getPieceOnAny(sq, who);

			result += toChar(pieceOn, who);
		}
	}

	return result;
}

std::string Position::print() {
	return printing::printBoard(charBB());
}