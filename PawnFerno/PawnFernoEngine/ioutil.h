#include "bitboard.h"
#include <iostream>
#include <string>
#include "misc.h"
#include "fen.h"

using namespace std;

namespace printing {
	string toBitString(BitBoard);
	void print(BitBoard);
	void print(Square);
	void printBoard(string);
}