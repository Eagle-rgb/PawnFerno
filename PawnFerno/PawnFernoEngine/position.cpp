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

	// Board
	std::string charBB = fen::toCharBB(misc::split(*it, '/'));

	// Goes through every square, reads the appropriate entry in the charBB and updates
	// the appropriate piece bitboard as well as player bitboard.
	for (Rank i = RANK1; i <= RANK8; ++i) {
		for (File j = FILEA; j <= FILEH; ++j) {
			Square sq = toSquare(j, i);

			char c = charBB[sq];

			if (c == ' ') { ++j; continue; }

			BitBoard current = toBB(sq);
			if (misc::isUpper(c)) BB_wb[WHITE] |= current;
			else BB_wb[BLACK] |= current;

			PieceType pt = toPieceType(c);
			BB_pieces[pt] |= current;
		}
	}

	++it;

	// Player to move.
	player = fen::sideToMove(*it);
	++it;

	// Castling rights.
	state -> castlingRights = (int)fen::castlingRights(*it);
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

	// TODO assert that there is actually a piece standing on originSquare and no piece on destinationSquare.
	movePiece(originSquare, destinationSquare, movedPiece, player);

	newState.capturedPiece = capturedPiece;
	newState.previousState = state;
	state = &newState;
	player = !player;
}

void Position::undoMove(const Move m) {
	Square destinationSquare = move::destinationSquare(m);
	Square originSquare = move::originSquare(m);
	PieceType movedPiece = getPieceOn(destinationSquare);
	PieceType capturedPiece = state -> capturedPiece;

	Color playerWhoMoved = !player;

	// Move the piece from the destination square to the origin square.
	movePiece(destinationSquare, originSquare, movedPiece, playerWhoMoved);

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

Position::~Position() {
	delete[] BB_wb;
	delete[] BB_pieces;
	delete state;
}