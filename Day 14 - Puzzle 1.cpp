#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


int main() {
	// Opens the file
	std::ifstream ifs{ "input.txt" };
	if (!ifs.is_open()) return -1;

	// Reads in the map
	std::vector<std::string> map, mapCopy;
	std::string line = "";
	while (std::getline(ifs, line)) {
		map.push_back(line);
	}
	mapCopy = map;

	// Runs the cellular automaton until the previous state is the
	// same as the current state
	do {
		map = mapCopy;
		for (size_t i = 1; i < map.size(); ++i) {
			for (size_t j = 0; j < map.at(0).size(); ++j) {
				// Ignore if it is a blank space or a rock fixed in place
				if (map.at(i).at(j) != 'O') {
					mapCopy.at(i).at(j) = map.at(i).at(j);
					continue;
				}

				// If it is a round stone
				if (map.at(i - 1).at(j) == '.') {
					mapCopy.at(i).at(j) = '.';
					mapCopy.at(i - 1).at(j) = 'O';
				}
			}
		}
	} while (map != mapCopy);

	// Calculates the total weight
	int sum = 0;
	for (size_t i = 0; i < map.size(); ++i) {
		sum += std::count(map.at(i).begin(), map.at(i).end(), 'O') * (map.size() - i);
	}

	std::cout << sum << std::endl;
	
	return 0;
}
