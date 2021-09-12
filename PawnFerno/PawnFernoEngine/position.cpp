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

Position::Position(string fen) {
	// Initialization, default values.
	clear();
	vector<string> fenParts = misc::split(fen, ' ');

	vector<string>::iterator it = fenParts.begin();

	// Board
	string charBB = fen::toCharBB(misc::split(*it, '/'));
	int i = 7;
	int j = 0;

	for (char c : charBB) {
		if (j == 8) { --i; j = 0; }
		if (c == ' ') { ++j; continue; }

		Square current = Square(8 * i + j);
		if (misc::isUpper(c)) BB_wb[WHITE] |= current;
		else BB_wb[BLACK] |= current;

		PieceType pt = toPieceType(c);
		BB_pieces[(int)pt] |= current;

		++j;
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