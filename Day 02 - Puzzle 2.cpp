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
	int sum = 0;

	// Variables to help with parsing
	std::string str = "";
	size_t index = 1;

	// Parses the input line by line
	while (std::getline(ifs, str)) {
		std::stringstream line{ str };
		std::string discard = ""; 
		line >> discard >> discard; // Discards the "Game [num]:" part
		
		// Keeps track of the minimum amount of cubes necessary for each color
		std::unordered_map<std::string, int> minColors = {
			{ "red", 0 },
			{ "green", 0 },
			{ "blue", 0 }
		};

		int num = 0;
		std::string color = "";
		while (line >> num >> color) {
			// Removes the useless comma or semicolon
			if (color.back() == ',' || color.back() == ';') color.pop_back();

			// Checks to see if the game is possible
			for (auto& minColor : minColors) {
				if (color == minColor.first && num > minColor.second) minColor.second = num;
			}
		}
		
		// Calculates the product and adds it to the sum
		int product = 1;
		for (auto& minColor : minColors) {
			product *= minColor.second;
		}
		sum += product;
	}

	std::cout << sum << std::endl;
}
