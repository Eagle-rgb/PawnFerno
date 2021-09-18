#pragma once

#ifndef MISC_INCLUDED
#define MISC_INCLUDED
#include <string>
#include <vector>

namespace misc {
	using namespace std;

	/// <summary>
	/// Splits the given string using the given separator.
	/// </summary>
	vector<string> split(const string&, char);

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

	/// <summary>
	/// Returns true if the given string contains the character.
	/// </summary>
	extern bool contains(string, char);
}
#endif