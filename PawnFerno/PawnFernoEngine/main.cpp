#include "main.h"


int main() {
	BitBoardInit();
	eval::init();
	uci::uci_loop();
}