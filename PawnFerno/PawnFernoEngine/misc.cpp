#include "misc.h"

namespace misc {
	vector<string> split(const string& input, char separator) {
		int length = input.length();
		vector<string> output;
		vector<char> current;

		for (int i = 0; i < length; i++)
		{
			if (input[i] != separator)
				current.push_back(input[i]);

			else {
				output.push_back(string(current.begin(), current.end()));
				current.clear();
			}
		}

		output.push_back(string(current.begin(), current.end()));

		return output;
	}

	bool contains(string TArr, char t) {
		for (auto tx : TArr) {
			if (t == tx) return true;
		}

		return false;
	}
}