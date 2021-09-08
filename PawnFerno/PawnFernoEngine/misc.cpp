#include "misc.h"

namespace misc {
	vector<string> split(string input, char separator) {
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
}