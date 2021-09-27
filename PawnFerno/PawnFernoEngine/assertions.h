#pragma once

#ifndef ASSERTIONS_INCLUDE
#define ASSERTIONS_INCLUDE

#include "types.h"
#include "move.h"
#include "bitboard.h"
#include <string>

namespace move {
	bool isAlgebraic(std::string m);
}

template <typename V, typename T> bool is_ok(T x);
template<> bool is_ok<File, char>(char);
template<> bool is_ok<Rank, char>(char);
template<> bool is_ok<PieceType, char>(char);
#endif