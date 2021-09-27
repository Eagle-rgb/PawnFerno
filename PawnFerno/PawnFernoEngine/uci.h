#pragma once

#ifndef UCI_INCLUDED
#define UCI_INCLUDED

#include "position.h"
#include "search.h"
#include <iostream>
#include <string>
#include "assertions.h"
#include <sstream>

namespace uci {
	const std::string engine_identification = "name PawnFerno author Pepegas";
	void uci_loop();
}
#endif