#include "position.h"

Position::Position() {
	BB_wb[WHITE] = BB_RANK1 | BB_RANK2;
	BB_wb[BLACK] = BB_RANK7 | BB_RANK8;

	BB_pieces[PAWN] = BB_RANK2 | BB_RANK7;
	BB_pieces[KNIGHT] = toBB(SQb1) | toBB(SQg1) | toBB(SQb8) | toBB(SQg8);
	BB_pieces[BISHOP] = toBB(SQc1) | toBB(SQc8) | toBB(SQf1) | toBB(SQf8);
	BB_pieces[ROOK] = toBB(SQa1) | toBB(SQa8) | toBB(SQh1) | toBB(SQh8);
	BB_pieces[QUEEN] = toBB(SQd1) | toBB(SQd8);
	BB_pieces[KING] = toBB(SQe1) | toBB(SQe8);

	state.player = WHITE;
	state.castlingRights = 15;
	state.enPassant = SQNONE;
}

Position::Position(std::string fen) {
	// Initialization, default values.
	clear();
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
	state.player = fen::sideToMove(*it);
	++it;

	// Castling rights.
	state.castlingRights = (int)fen::castlingRights(*it);
	++it;

	// En Passant square.
	state.enPassant = fen::enPassant(*it);
	++it;


	// Half move clock. TODO

	// Full move clock. TODO
}

void Position::clear() {
	BB_wb[0] = BB_EMPTY; BB_wb[1] = BB_EMPTY;

	for (PieceType i = PAWN; i <= KING; ++i) {
		BB_pieces[i] = BB_EMPTY;
	}

	state.castlingRights = 0;
	state.enPassant = SQNONE;
}

PieceType Position::getPieceOn(Square sq) {
	return getPieceOn(sq, state.player);
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

void Position::makeMove(const Move m) {
	Square originSquare = move::originSquare(m);
	Square destinationSquare = move::destinationSquare(m);
	PieceType movedPiece = getPieceOn(originSquare);
	PieceType capturedPiece = getPieceOn(destinationSquare);

	// TODO assert that there is actually a piece standing on originSquare and no piece on destinationSquare.
	BB_wb[state.player] ^= (toBB(originSquare) | toBB(destinationSquare));
	BB_pieces[movedPiece] ^= (toBB(originSquare) | toBB(destinationSquare));
}

void Position::undoMove(const Move m) {
	Square destinationSquare = move::destinationSquare(m);
	Square originSquare = move::originSquare(m);
	PieceType movedPiece = getPieceOn(destinationSquare);
	PieceType capturedPiece = 
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