#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


// Calculates the next values by finding discrete derivates
// and recursion
int prevValue(const std::vector<int> values) {
	// Calculates the discrete derivative
	std::vector<int> derivative;
	for (size_t i = 0; i < values.size() - 1; ++i) {
		derivative.push_back(values.at(i + 1) - values.at(i));
	}

	// If the derivative is just zeros then return the same value
	if (derivative == std::vector<int>(derivative.size(), 0)) return values.front();

	// Otherwise just repeat this process until we get the 0 derivative
	return values.front() - prevValue(derivative);
}


int main() {
	// Opens the file
	std::ifstream ifs{ "input.txt" };
	if (!ifs.is_open()) return -1;

	// Reads and process the input line by line
	int sum = 0;
	std::string line = "";
	while (std::getline(ifs, line)) {
		// Reads in the values on the line
		std::stringstream lineStream{ line };
		std::vector<int> values;
		int value = 0;
		while (lineStream >> value) values.push_back(value);

		// Gets the next value and adds it to the sum
		sum += prevValue(values);
	}

	std::cout << sum << std::endl;

	return 0;
}
