#pragma once

#ifndef MISC_INCLUDED
#define MISC_INCLUDED
#include <string>
#include <vector>

using namespace std;

namespace misc {
	vector<string> split(string, char);

	constexpr bool isNumeric(char c) {
		return '0' <= c && '9' >= c;
	}

	constexpr bool isLower(char c) {
		return 'a' <= c && 'z' >= c;
	}

	constexpr bool isUpper(char c) {
		return 'A' <= c && 'Z' >= c;
	}

	constexpr bool isLetter(char c) {
		return 'A' <= c && 'z' >= c;
	}

	extern bool contains(string, char);
}
#endif