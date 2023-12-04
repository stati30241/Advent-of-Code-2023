#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <sstream>


int main() {
	// Opens the file
	std::ifstream ifs{ "input.txt" };
	if (!ifs.is_open()) return -1;

	// Parses each card line by line
	int sum = 0;
	std::string line = "";
	while (std::getline(ifs, line)) {
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

		sum += numbersWon ? std::pow(2, numbersWon - 1) : 0;
	}

	std::cout << sum << std::endl;

	return 0;
}
