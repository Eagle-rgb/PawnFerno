#include "main.h"


int main() {
	BitBoardInit();
	eval::init();
	hashing::init();
	uci::uci_loop();
}