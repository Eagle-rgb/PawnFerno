#include "bitboard.h"
#include <string>
#include "misc.h"
#include "fen.h"

namespace printing {
	std::string toBitString(BitBoard);
	std::string print(BitBoard);
	std::string print(Square);
	std::string printBoard(std::string);
}