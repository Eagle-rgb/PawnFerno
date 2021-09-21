#pragma once

#ifndef PERFT_INCLUDED
#define PERFT_INCLUDED

#include <string>
#include "position.h"
#include "ioutil.h"

void perft(std::string fen, int depth);
void perft(Position* p, int depth);
int perftRec(Position* p, int depth, bool first);

#endif