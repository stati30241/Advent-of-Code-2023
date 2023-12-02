#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>


int main() {
	// Opens the file
	std::ifstream ifs{ "input.txt" };
	if (!ifs.is_open()) return -1;

	// Output
	std::unordered_map<std::string, int> maxColors = {
		{ "red", 12 },
		{ "green", 13 },
		{ "blue", 14 }
	};
	int sum = 0;

	// Variables to help with parsing
	std::string str = "";
	size_t index = 1;

	// Parses the input line by line
	while (std::getline(ifs, str)) {
		std::stringstream line{ str };
		std::string discard = ""; 
		line >> discard >> discard; // Discards the "Game [num]:" part

		bool possible = true;
		int num = 0;
		std::string color = "";
		while (line >> num >> color) {
			// Removes the useless comma or semicolon
			if (color.back() == ',' || color.back() == ';') color.pop_back();

			// Checks to see if the game is possible
			for (const auto& maxColor : maxColors) {
				if (color == maxColor.first && num > maxColor.second) possible = false;
			}
		}

		if (possible) sum += index;
		index++;
	}

	std::cout << sum << std::endl;
}
