#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <sstream>
#include <vector>


int main() {
	// Opens the file
	std::ifstream ifs{ "input.txt" };
	if (!ifs.is_open()) return -1;

	// Parses each card line by line
	int sum = 0;
	std::vector<int> copies = { 0 };
	std::string line = "";
	for (size_t index = 0; std::getline(ifs, line); ++index) {
		std::stringstream lineStream{ line };
		std::string discard = "";
		lineStream >> discard >> discard;

		// Parses the numbers on the cards
		std::string token = "";
		std::set<int> winningNumbers;
		bool parsingWinningNumbers = true;
		int numbersWon = 0;
		while (lineStream >> token) {
			// If we encouter the seperation
			if (token == "|") {
				parsingWinningNumbers = false;
				continue;
			}

			// If we are still parsing the winning numbers
			if (parsingWinningNumbers) winningNumbers.insert(std::atoi(token.c_str()));

			// Otherwise we are checking if the numbers we
			// have are winning numbers or not
			else if (winningNumbers.contains(std::atoi(token.c_str()))) numbersWon++;
		}
		
		// Notes down how many copies are made of each of the cards
		if (copies.size() < index + numbersWon) copies.resize(copies.size() + numbersWon - index, 0);
		for (size_t i = 1; i <= numbersWon; ++i) {
			copies.at(index + i) += copies.at(index) + 1;
		}
		copies.push_back(0); // Adds random 0s to the end so we never are out of range

		// Adds how many copies of cards we have to the sum
		sum += copies.at(index) + 1;
	}

	std::cout << sum << std::endl;

	return 0;
}
