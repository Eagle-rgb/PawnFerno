#pragma once

#ifndef MOVE_INCLUDED
#define MOVE_INCLUDED

#include "types.h"
#include "bitboard.h"
#include <vector>
#include <string>

namespace move {
	enum SpecialMoves {
		normal = 0,
		enPassant = 0b1 << 12,
		castling = 0b10 << 12,
		promotion = 0b11 << 12
	};

	// We do not want to use the directly outside of this class - that's why they are static.
	static constexpr short originSquareBits = 0b111111;
	static constexpr short promotionPieceShift = 14;

	/// <summary>
	/// Constructs a move using an origin and a destination square.
	/// No en passant
	/// No castling
	/// No promotion
	/// </summary>
	constexpr Move makeMove(const Square origin, const Square destination) {
		return (short)origin | (short)(destination << 6);
	}

	constexpr Move makeEnPassantMove(const Square origin, const Square destination) {
		return (short)origin | (short)(destination << 6) | enPassant;
	}

	constexpr Move makeCastlingMove(const Color who, const SCastling c) {
		return (short)(castlingKingOrigin(who)) | (short)(castlingKingDestination(who, c) << 6) | castling;
	}

	constexpr Move makePromotionMove(const Square origin, const Square destination, const PieceType promotionPiece) {
		return (short)origin | (short)(destination << 6) | promotion | ((short)promotionPiece - 1 << promotionPieceShift);
	}

	/// <summary>
	/// Returns the origin square (first 5 bits) of the given move.
	/// </summary>
	constexpr Square originSquare(const Move& m) {
		return Square(m & originSquareBits);
	}

	/// <summary>
	/// Returns the destination square (bits 6 - 10) of the given move.
	/// </summary>
	constexpr Square destinationSquare(const Move& m) {
		return originSquare(m >> 6);
	}

	constexpr SCastling castlingTypeFromMove(const Move& m) {
		if ((toBB(destinationSquare(m)) & BB_FILEG) != BB_EMPTY)
			return SCastling::K;

		return SCastling::Q;
	}

	constexpr PieceType getPromotionPiece(const Move& m) {
		short piece = m >> promotionPieceShift;

		return PieceType(piece + 1);
	}

	constexpr bool isEnPassant(const Move& m) { return ((short)m & promotion) == enPassant; }

	constexpr bool isCastling(const Move& m) { return ((short)m & promotion) == castling; }

	constexpr bool isPromotion(const Move& m) { return ((short)m & promotion) == promotion; }

	std::vector<Move> makeNormalMoves(const Square from, BitBoard destinations);

	std::vector<Move> makePromotions(const Square from, BitBoard destinations);
}

#endif