#include "assertions.h"

using namespace std;



namespace move {
	bool isAlgebraic(string m) {
		if (m.size() < 4 || m.size() > 5) return false;
		if (!(is_ok<File>(m[0]) && is_ok<Rank>(m[1]) && is_ok<File>(m[2]) && is_ok<Rank>(m[3]))) return false;
		if (m.size() == 5 && !is_ok<PieceType>(m[4])) return false;
		return true;
	}
}

template<> bool is_ok<File, char>(char x) {
	return 'a' <= x && 'h' >= x;
}

template<> bool is_ok<Rank, char>(char x) {
	return '1' <= x && '8' >= x;
}

template<> bool is_ok<PieceType, char>(char x) {
	x += x < 'a' ? 32 : 0;  // make x lowercase.

	return x == 'p' || x == 'n' || x == 'b' || x == 'r' || x == 'q' || x == 'k';
}