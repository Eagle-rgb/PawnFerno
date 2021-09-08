#pragma once

#ifndef FEN_INCLUDED
#define FEN_INCLUDED

#include "misc.h"
#include <assert.h>

using namespace std;
constexpr char pieceChars[12] = { 'K', 'Q', 'R', 'B', 'N', 'P', 'k', 'q', 'r', 'b', 'n', 'p' };

string toCharBB(vector<string> boardParts);
#endif