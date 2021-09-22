#pragma once

#ifndef MOVE_INCLUDED
#define MOVE_INCLUDED

#include "types.h"
#include "bitboard.h"
#include <vector>
#include <string>

namespace move {
	enum SpecialMoves {
		enPassant = 0b1 << 12,
		castling = 0b10 << 12,
		promotion = 0b11 << 12
	};

	constexpr short originSquareBits = 0b111111;
	constexpr short promotionPieceShift = 14;

	/// <summary>
	/// Constructs a move using an origin and a destination square.
	/// No en passant
	/// No castling
	/// No promotion
	/// </summary>
	constexpr Move makeMove(Square origin, Square destination) {
		return (short)origin | (short)(destination << 6);
	}

	constexpr Move makeEnPassantMove(Square origin, Square destination) {
		return (short)origin | (short)(destination << 6) | enPassant;
	}

	constexpr Move makeCastlingMove(Color who, SCastling c) {
		return (short)(castlingKingOrigin(who)) | (short)(castlingKingDestination(who, c) << 6) | castling;
	}

	constexpr Move makePromotionMove(Square origin, Square destination, PieceType promotionPiece) {
		return (short)origin | (short)(destination << 6) | promotion | ((short)promotionPiece - 1 << promotionPieceShift);
	}

	/// <summary>
	/// Returns the origin square (first 5 bits) of the given move.
	/// </summary>
	constexpr Square originSquare(Move m) {
		return Square(m & originSquareBits);
	}

	/// <summary>
	/// Returns the destination square (bits 6 - 10) of the given move.
	/// </summary>
	constexpr Square destinationSquare(Move m) {
		return originSquare(m >> 6);
	}

	constexpr SCastling castlingTypeFromMove(Move m) {
		if ((toBB(destinationSquare(m)) & BB_FILEG) != BB_EMPTY)
			return SCastling::K;

		return SCastling::Q;
	}

	constexpr PieceType getPromotionPiece(Move m) {
		short piece = m >> promotionPieceShift;

		return PieceType(piece + 1);
	}

	constexpr bool isEnPassant(Move m) { return ((short)m & promotion) == enPassant; }

	constexpr bool isCastling(Move m) { return ((short)m & promotion) == castling; }

	constexpr bool isPromotion(Move m) { return ((short)m & promotion) == promotion; }

	std::vector<Move> makeNormalMoves(Square from, BitBoard destinations);

	std::vector<Move> makePromotions(Square from, BitBoard destinations);
}

#endif