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

	player = WHITE;
	castlingRights = 15;
	enPassant = SQNONE;
}

Position::Position(std::string fen) {
	// Initialization, default values.
	clear();
	std::vector<std::string> fenParts = misc::split(fen, ' ');

	std::vector<std::string>::iterator it = fenParts.begin();

	// Board
	std::string charBB = fen::toCharBB(misc::split(*it, '/'));

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			char c = charBB[8 * i + j];

			if (c == ' ') { ++j; continue; }

			BitBoard current = toBB(Square(8 * i + j));
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
	castlingRights = (int)fen::castlingRights(*it);
	++it;

	// En Passant square.
	enPassant = fen::enPassant(*it);
	++it;


	// Half move clock. TODO

	// Full move clock. TODO
}

void Position::clear() {
	BB_wb[0] = BB_EMPTY; BB_wb[1] = BB_EMPTY;

	for (int i = 0; i < 6; i++) {
		BB_pieces[i] = BB_EMPTY;
	}

	castlingRights = 0;
	enPassant = SQNONE;
}

PieceType Position::getPieceOn(Square sq) {
	return getPieceOn(sq, player);
}

PieceType Position::getPieceOn(Square sq, Color who) {
	BitBoard currentPlayerBB = BB_wb[who];

	for (int i = 0; i < 6; i++)
		if (!isFree(BB_pieces[i] & currentPlayerBB, sq)) return PieceType(i);

	return PIECENONE;
}

PieceType Position::getPieceOnAny(Square sq, Color& c) {
	for (int i = 0; i < 6; i++) {
		if (!isFree(BB_pieces[i], sq)) {
			c = !isFree(BB_wb[WHITE], sq) ? WHITE : BLACK;
			return PieceType(i);
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
	BB_wb[player] ^= (toBB(originSquare) | toBB(destinationSquare));
	BB_pieces[movedPiece] ^= (toBB(originSquare) | toBB(destinationSquare));
}

std::string Position::charBB() {
	std::string result = "";

	for (int i = 7; i >= 0; i--) {
		for (int j = 0; j < 8; j++) {
			Color who = WHITE;
			Square sq = toSquare(File(j), Rank(i));
			PieceType pieceOn = getPieceOnAny(sq, who);

			result += toChar(pieceOn, who);
		}
	}

	return result;
}

std::string Position::print() {
	return printing::printBoard(charBB());
}