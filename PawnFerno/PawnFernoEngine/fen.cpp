#include "fen.h"

namespace fen {
	string toCharBB(vector<string> boardParts) {
		assert(boardParts.size() == 8);

		char board[64];

		vector<string>::iterator it;
		int i = 0;

		for (int i = 0; i < 64; i++) {
			board[i] = ' ';
		}

		for (it = boardParts.begin(); it != boardParts.end(); ++it) {
			int j = 0;
			string row = *it;

			for (char elem : row) {
				if ('A' <= elem && elem <= 'z') {
					board[(7 - i) * 8 + j] = elem;
					++j; continue;
				}

				else if ('0' <= elem && elem <= '9') {
					j += elem - 48; continue;
				}

				throw new exception("Invalid board parts.");
			}

			++i;
		}

		return board;
	}

	Color sideToMove(string s) {
		char colorChar = s[0];

		return colorChar == 'w' ? WHITE : (colorChar == 'b' ? BLACK : throw new exception("Invalid color in fen"));
	}

	Castling castlingRights(string c) {
		assert(c.length() <= 4);

		if (c == "-") return Castling::None;
		assert(c.length() > 0);

		Castling result = Castling::None;

		for (char cx : c) {
			assert(misc::contains(castlingChars, cx)); // else the castling part of the fen includes an illegal character.
			result += (int)static_cast<Castling>(cx);
		}

		return result;
	}

	Square enPassant(string s) {
		// No en passant square if just '-'
		if (s == "-") return SQNONE;

		// Now, first we check if the square is valid.
		assert(s.length() == 2);
		assert('a' <= s[0] && 'h' >= s[0]);
		assert('1' <= s[1] && '8' >= s[1]);

		// Is valid, we can calculate an actual square.
		Rank rank = Rank(s[0] - 'a');
		File file = File(s[1] - '1');

		return toSquare(file, rank);
	}

	int halfClock(string s) {
		for (char c : s) assert(misc::isNumeric(c));
		return stoi(s);
	}

	int fullClock(string s) {
		return halfClock(s);
	}
}