#include "fen.h"

string toCharBB(vector<string> boardParts) {
	assert(boardParts.size() == 8);

	char board[64];

	for (int i = 0; i < 64; i++) {
		board[i] = ' ';
	}

	for (int i = 0; i < 8; i++) {
		int j = 0;
		string row = boardParts[i];

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
	}

	return board;
}