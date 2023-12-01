#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


int main() {
	// Opens the input
	std::ifstream ifs{ "input.txt" };
	if (!ifs.is_open()) return -1;

	// Contains all the digits accessible throught their index
	std::vector<std::string> digits
		= { "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };
	int sum = 0;

	std::string line = "";
	while (ifs >> line) {
		// Searches for the first number
		std::string token = "";
		bool found = false;
		for (char ch : line) {
			if (found) break;

			// If the character is just a number
			if (ch >= 48 && ch <= 57) {
				sum += (ch - 48) * 10;
				found = true;
			}

			// Otherwise search for spelled out numbers
			token += ch;
			for (size_t i = 0; i < digits.size(); ++i) {
				size_t index = token.find(digits.at(i));
				if (index == std::string::npos) continue;
				sum += i * 10;
				found = true;
			}
		}

		// Searches for the last number (backwards)
		std::reverse(line.begin(), line.end());
		token = "";
		found = false;
		for (char ch : line) {
			if (found) break;

			// If the character is just a number
			if (ch >= 48 && ch <= 57) {
				sum += (ch - 48);
				found = true;
			}

			// Otherwise search for spelled out numbers
			token = ch + token; // Have to concatanate to the front
			for (size_t i = 0; i < digits.size(); ++i) {
				size_t index = token.find(digits.at(i));
				if (index == std::string::npos) continue;
				sum += i;
				found = true;
			}
		}
	}

	std::cout << sum << std::endl;

	return 0;
}
