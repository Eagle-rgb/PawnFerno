#pragma once

#ifndef IOUTIL_INCLUDE
#define IOUTIL_INCLUDE

#include "bitboard.h"
#include <string>
#include "misc.h"
#include "fen.h"
#include "move.h"
#include "position.h"
#include <iostream>

/*inline std::ostream& operator<<(std::ostream& os, const BitBoard& bb) {
	os << printing::print(bb); return os;
}*/

namespace printing {
	/// <summary>
	/// Prints the 0's and 1's of the 64 Bit BitBoard.
	/// </summary>
	std::string toBitString(BitBoard);

	/// <summary>
	/// Prints the BitBoard in a matrix fashion.
	/// </summary>
	std::string print(BitBoard);

	/// <summary>
	/// Prints the square.
	/// </summary>
	std::string print(Square);

	std::string print(Move, Color);

	std::string print(Move);

	std::string print(std::vector<Move>, Color);
	
	std::string printSquareBB(Square);

	/// <summary>
	/// Prints a given charBB in a fancy way. (see Position::charBB)
	/// </summary>
	std::string printBoard(std::string);
}

namespace move {
	/// <summary>
	/// Converts the algebraic notation of the move into an actual move object.
	/// This will only yield a pseudo move, and has to be confirmed in the position it shall be played in.
	/// </summary>
	Move makeMove(std::string);
}

#endif IOUTIL_INCLUDE