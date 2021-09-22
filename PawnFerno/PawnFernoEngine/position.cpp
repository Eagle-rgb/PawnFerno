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

			if (c == ' ') continue;

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

Color Position::playerToMove() {
	return player;
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

BitBoard Position::getAttacksOf(Color who, bool excludeKing) {
	BitBoard piecesOf[6];
	BitBoard blockers = BB_wb[0] | BB_wb[1];

	if (excludeKing) blockers &= ~(BB_wb[!who] & BB_pieces[KING]);

	for (PieceType p = PAWN; p <= KING; ++p) {
		piecesOf[p] = BB_pieces[p] & BB_wb[who];
	}

	return attacksOfAll(piecesOf, blockers, who);
}

BitBoard Position::getEnemyAttacks() {
	BitBoard enemyPieces[6];
	BitBoard blockers = BB_wb[0] | BB_wb[1];

	blockers &= ~(BB_wb[player] & BB_pieces[KING]);

	for (PieceType p = PAWN; p <= KING; ++p) {
		enemyPieces[p] = BB_pieces[p] & BB_wb[!player];
	}

	state->moveGenCheck |= generatedEnemyAttacks;
	return (state->enemyAttacks = attacksOfAll(enemyPieces, blockers, !player));
}

void Position::makePinnersAndCheckers() {
	Square kingSquare = toSquare(BB_wb[player] & BB_pieces[KING]);
	//BitBoard pinnerRays = BB_EMPTY;
	BitBoard pinnedPieces = BB_EMPTY;
	BitBoard checkingPieces = BB_EMPTY;

	// Remember that only sliders can pin.

	for (PieceType p = BISHOP; p <= QUEEN; ++p) {
		BitBoard sliders = BB_wb[!player] & BB_pieces[p];
		Square sq = SQNONE;

		while ((sq = bits::popLSB(sliders)) != SQNONE) {
			Direction toKing = relativeDirection(sq, kingSquare);

			// Abort if this piece can not attack in that direction.
			if (p == BISHOP && (abs(toKing) == EAST || abs(toKing) == NORTH)) continue;
			else if (p == ROOK && (abs(toKing) == NORTHWEST || abs(toKing) == NORTHEAST)) continue;

			// Shoot ray from the piece to the king and backwards and see if they meet.
			BitBoard shotToKing = RAYS[sq][directionIndex(toKing)];
			BitBoard shotFromKing = RAYS[kingSquare][directionIndex(-toKing)] | toBB(kingSquare);

			if (isEmpty(shotToKing & shotFromKing)) continue;

			// No enemy piece may be in the line of sight of the pinner.
			if (!isEmpty(shotFromKing & shotToKing & BB_wb[!player])) continue;

			BitBoard between = shotToKing & shotFromKing & BB_wb[player] & ~toBB(kingSquare);

			// There can only be none or at most one piece in the line of sight.
			if (more_than_one(between)) continue;

			// If no piece is in the way this even is a checking piece (awesome :)).
			if (isEmpty(between)) checkingPieces |= toBB(sq);

			// Finally, this is a checking piece or a pinner (we will save the ray in both cases).
			// Doing it this way will include the pinner's square and the king square.
			//pinnerRays |= toBB(sq) | (shotToKing ^ RAYS[kingSquare][directionIndex(toKing)]);
			pinnedPieces |= between;
		}

	}

	Square sq = SQNONE;
	BitBoard enemyPawns = BB_pieces[PAWN] & BB_wb[!player];

	while ((sq = bits::popLSB(enemyPawns)) != SQNONE) {
		if (!isEmpty(PAWN_CAPTURES[!player][sq] & toBB(kingSquare)))
			checkingPieces |= toBB(sq);
	}

	BitBoard enemyKnights = BB_pieces[KNIGHT] & BB_wb[!player];

	while ((sq = bits::popLSB(enemyKnights)) != SQNONE) {
		if (!isEmpty(KNIGHT_ATTACKS[sq] & toBB(kingSquare)))
			checkingPieces |= toBB(sq);
	}

	state->checkingPieces = checkingPieces;
	state->pinnedPieces = pinnedPieces;
	state->moveGenCheck |= generatedCheckingPieces | generatedPinnedPieces;
}

std::vector<Move> Position::getLegalPawnMoves(BitBoard checkRay) {
	BitBoard pawnBB = playerPieceBBof(PAWN);
	BitBoard blockers = blockerBB();
	Square sq = SQNONE;
	std::vector<Move> moves;

	// Pawn moves
	while ((sq = bits::popLSB(pawnBB)) != SQNONE) {
		BitBoard moveBB = PAWN_CAPTURES[player][sq] & BB_wb[!player];
		BitBoard enPassantMoveBB = BB_EMPTY;

		// Very compact way to determine if we can push our pawn, and if yes, then to which square.
		moveBB |= PAWN_PUSHES[player][sq] & ~(blockers | (shift(blockers ^ toBB(sq), PAWN_DIRECTIONS[player])));

		if (isPinned(sq)) {
			moveBB &= RAYS[kingSquare()][directionIndex(relativeDirection(kingSquare(), sq))];
		}

		moveBB &= checkRay;

		if (canPromote(player, sq)) {
			std::vector<Move> pawnMoves = move::makePromotions(sq, moveBB);
			moves.insert(moves.end(), pawnMoves.begin(), pawnMoves.end());
			continue;
		}

		std::vector<Move> pawnMoves = move::makeNormalMoves(sq, moveBB);

		// Can en passant.
		if (state->enPassant != SQNONE && !isEmpty(PAWN_CAPTURES[player][sq] & toBB(state->enPassant))) {
			Move enPassant = move::makeEnPassantMove(sq, state->enPassant);
			if (tryMove(enPassant)) {
				pawnMoves.push_back(move::makeEnPassantMove(sq, state->enPassant));
			}
		}
			
		moves.insert(moves.end(), pawnMoves.begin(), pawnMoves.end());
	}

	return moves;
}

std::vector<Move> Position::getLegalSliderMoves(BitBoard checkRay) {
	BitBoard blockers = blockerBB();
	Square sq;

	std::vector<Move> moves;

	for (PieceType p = KNIGHT; p <= QUEEN; ++p) {
		BitBoard pieceBB = playerPieceBBof(p);
		while ((sq = bits::popLSB(pieceBB)) != SQNONE) {
			BitBoard moveBB = attacksOf(sq, blockers, player, p);

			// First mask. We can not capture our own pieces.
			moveBB &= ~BB_wb[player];

			// Second mask. We can only move on the pin ray.
			if (isPinned(sq)) {
				moveBB &= RAYS[kingSquare()][directionIndex(relativeDirection(kingSquare(), sq))];
			}

			moveBB &= checkRay;

			std::vector<Move> pieceMoves = move::makeNormalMoves(sq, moveBB);

			moves.insert(moves.end(), pieceMoves.begin(), pieceMoves.end());
		}
	}

	return moves;
}

std::vector<Move> Position::getLegalCastlings() {
	std::vector<Move> moves;
	short castlingRights = state->castlingRights;

	// We do not care if the squares b1 or b8 are under attack. We can castle nontheless.
	// In general, this means we can disregard the B File.
	BitBoard enemyAttacks = state->enemyAttacks &~ BB_FILEB;
	BitBoard blockers = enemyAttacks | blockerBB();

	short kingSideCastling = (short)Castling::K << ((short)player * 2);
	short queenSideCastling = (short)Castling::Q << ((short)player * 2);

	if (castlingRights & kingSideCastling && isEmpty(blockers & castlingPath(player, SCastling::K)))
		moves.push_back(move::makeCastlingMove(player, SCastling::K));

	if (castlingRights & queenSideCastling && isEmpty(blockers & castlingPath(player, SCastling::Q)))
		moves.push_back(move::makeCastlingMove(player, SCastling::Q));
	
	return moves;
}

std::vector<Move> Position::getLegalMoves() {
	assert((state->moveGenCheck & 0b111) == 0b111);

	BitBoard blockers = blockerBB();
	Square kingSquare = toSquare(BB_wb[player] & BB_pieces[KING]);
	Square sq = SQNONE;  // iterator square.

	std::vector<Move> moves;

	if (!inDoubleCheck()) {
		BitBoard checkRay = BB_FULL;

		if (inCheck()) {
			Square checkingPiece = toSquare(state->checkingPieces);
			checkRay = drawLine(kingSquare, checkingPiece) | state->checkingPieces;
		}

		std::vector<Move> pawnMoves = getLegalPawnMoves(checkRay);
		std::vector<Move> sliderMoves = getLegalSliderMoves(checkRay);

		moves.insert(moves.end(), pawnMoves.begin(), pawnMoves.end());
		moves.insert(moves.end(), sliderMoves.begin(), sliderMoves.end());
	}

	// King moves.
	BitBoard moveBB = KING_ATTACKS[kingSquare];
	moveBB &= ~state->enemyAttacks;
	moveBB &= ~BB_wb[player];
	std::vector<Move> kingMoves = move::makeNormalMoves(kingSquare, moveBB);
	moves.insert(moves.end(), kingMoves.begin(), kingMoves.end());

	// Castlings.
	if (!inCheck()) {
		std::vector<Move> castlings = getLegalCastlings();
		moves.insert(moves.end(), castlings.begin(), castlings.end());
	}

	state->legalMoves = moves;
	state->moveGenCheck |= generatedLegalMoves;

	return moves;
}

std::vector<Move> Position::getLegalMovesAuto() {
	getEnemyAttacks();
	makePinnersAndCheckers();
	return getLegalMoves();
}

bool Position::inCheck() {
	assert((state->moveGenCheck & generatedEnemyAttacks) != 0);
	return !isEmpty(BB_wb[player] & BB_pieces[KING] & state->enemyAttacks);
}

bool Position::inDoubleCheck() {
	assert((state->moveGenCheck & generatedEnemyAttacks) != 0);
	return more_than_one(BB_wb[player] & BB_pieces[KING] & state->enemyAttacks);
}

bool Position::isPinned(Square sq) {
	assert((state->moveGenCheck & generatedPinnedPieces) != 0);
	return !isFree(state->pinnedPieces, sq);
}

bool Position::tryMove(Move m) {
	State temp = State();

	makeMove(m, temp);
	BitBoard ourAttacks = getAttacksOf(player);

	if (!isEmpty(ourAttacks & BB_wb[!player] & BB_pieces[KING])) {
		undoMove(m);
		return false;
	}

	undoMove(m);
	return true;
}

bool Position::isCaptures(Move m) {
	BitBoard enemies = BB_wb[!player];
	BitBoard destination = BitBoard(move::destinationSquare(m));

	return (!isEmpty(enemies & destination));
}

void Position::movePiece(Square origin, Square destination, PieceType piece, Color who) {
	BitBoard orDesBB = toBB(origin) | toBB(destination);

	BB_wb[who] ^= orDesBB;
	BB_pieces[piece] ^= orDesBB;
}

void Position::removePiece(Square sq, PieceType piece, Color who) {
	BB_wb[who] ^= toBB(sq);
	BB_pieces[piece] ^= toBB(sq);
}

void Position::addPiece(Square sq, PieceType piece, Color who) {
	BB_wb[who] |= toBB(sq);
	BB_pieces[piece] |= toBB(sq);
}

void Position::makeMove(const Move m, State& newState) {
	Square originSquare = move::originSquare(m);
	Square destinationSquare = move::destinationSquare(m);
	PieceType movedPiece = getPieceOn(originSquare);
	PieceType capturedPiece = getPieceOn(destinationSquare, !player);

	newState.previousState = state;
	state = &newState;

	state->castlingRights = state->previousState->castlingRights;
	state->enPassant = SQNONE;
	state->moveGenCheck = 0;

	if (capturedPiece != PIECENONE) {
		removePiece(destinationSquare, capturedPiece, !player);
	}

	if (movedPiece == PAWN) {
		if (move::isEnPassant(m)) {
			removePiece(enPassantPawnSquare(destinationSquare, player), PAWN, !player);
		}

		// Check if it was a double push -> update enPassant state.
		if (squareDistance(originSquare, destinationSquare) >= 2) {
			state->enPassant = enPassantToSquare(destinationSquare, player);
		}

		// Check for promotion.
		if (move::isPromotion(m)) {
			PieceType promotionPiece = move::getPromotionPiece(m);

			addPiece(destinationSquare, promotionPiece, player);
			removePiece(originSquare, PAWN, player);
		}
	}

	// Check if it was a double push -> update enPassant state.

	// If king move, then remove castling rights.
	if (movedPiece == KING) {
		// Only the other player has his castling rights.
		state->castlingRights &= 0b11 << (2 * (short)!player);

		if (move::isCastling(m)) {
			// Move the rook.
			
			SCastling castlingType = move::castlingTypeFromMove(m);
			Square rookOrigin = castlingRookOrigin(player, castlingType);
			Square rookDestination = castlingRookDestination(player, castlingType);

			movePiece(rookOrigin, rookDestination, ROOK, player);
		}
	}

	// Remove castling rights for the appropriate origin square.
	if (movedPiece == ROOK) {
		if (originSquare == castlingRookOrigin(player, SCastling::K)) {
			state->castlingRights &= ~((short)SCastling::K << 2 * (short)player);
		}

		if (originSquare == castlingRookOrigin(player, SCastling::Q)) {
			state->castlingRights &= ~((short)SCastling::Q << 2 * (short)player);
		}
	}

	// Remove castling for the enemy if one of the enemy rook squares is "captured".
	if (destinationSquare == castlingRookOrigin(!player, SCastling::K)) {
		state->castlingRights &= ~((short)SCastling::K << 2 * (short)!player);
	}

	if (destinationSquare == castlingRookOrigin(!player, SCastling::Q)) {
		state->castlingRights &= ~((short)SCastling::Q << 2 * (short)!player);
	}

	// TODO assert that there is actually a piece standing on originSquare and no piece on destinationSquare.

	// We dont want to move the pawn if it is a promotion.
	if (!move::isPromotion(m))
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

	if (move::isEnPassant(m)) {
		addPiece(enPassantPawnSquare(destinationSquare, playerWhoMoved), PAWN, player);
	}

	if (move::isCastling(m)) {
		SCastling castlingType = move::castlingTypeFromMove(m);
		Square rookDestination = castlingRookDestination(playerWhoMoved, castlingType);
		Square rookOrigin = castlingRookOrigin(playerWhoMoved, castlingType);

		movePiece(rookDestination, rookOrigin, ROOK, playerWhoMoved);
	}

	if (move::isPromotion(m)) {
		// Trick:: Remove the promotion piece, add a pawn instead and then we will move the pawn afterwards.
		PieceType promotionPiece = move::getPromotionPiece(m);
		movedPiece = PAWN;

		removePiece(destinationSquare, promotionPiece, playerWhoMoved);
		addPiece(destinationSquare, PAWN, playerWhoMoved);
	}

	// Move the piece from the destination square to the origin square.
	movePiece(destinationSquare, originSquare, movedPiece, playerWhoMoved);

	// Add the captured piece back to the board.
	if (capturedPiece != PIECENONE)
		addPiece(destinationSquare, capturedPiece, player);

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