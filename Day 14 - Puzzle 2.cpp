#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


// Tilts the platform in the given direction
void tiltMap(std::vector<std::string>& map, const std::pair<int, int>& dir) {
	for (int line = 0; line < map.size(); ++line) {
		// Changes i based on the direction
		int i = (dir.first < 0) ? line : map.size() - line - 1;

		for (int column = 0; column < map.at(i).size(); ++column) {
			// Changes j based on the direction
			int j = (dir.second < 0) ? column : map.at(i).size() - column - 1;

			// We don't care if its not a round stone
			if (map.at(i).at(j) != 'O') continue;

			for (auto [x, y] = std::make_pair(i + dir.first, j + dir.second);; x+= dir.first, y+= dir.second) {
				// Break out if we are going out of bounds of the map
				if (x < 0 || x > map.size() - 1 || y < 0 || y > map.at(i).size() - 1) break;

				// Break out if isn't a free space
				if (map.at(x).at(y) != '.') break;

				// Otherwise move the round stone
				map.at(x - dir.first).at(y - dir.second) = '.';
				map.at(x).at(y) = 'O';
			}
		}
	}
}


// Checks if the pattern of the sums is repeating and returns the
// pattern start and pattern length
std::pair<int, int> isPatternRepeating(const std::vector<int>& sums) {
	// Random threshold value so two same values next to each other aren't
	// counted as a cycle, kind of a hack, but it works
	if (sums.size() < 2) return std::make_pair(-1, -1); 

	for (int i = 0; i < static_cast<int>(sums.size()) - 10; ++i) {
		// We need twice the cycle because we will be cutting it in half
		if ((sums.size() - i) % 2 == 1) continue;

		// Compares the first and second half not considering everything up to i
		std::vector<int> firstHalf{ sums.begin() + i, sums.begin() + i + (sums.size() - i) / 2 };
		std::vector<int> secondHalf{ sums.begin() + i + (sums.size() - i) / 2, sums.end() };
		if (firstHalf == secondHalf) {
			return std::make_pair(i, (sums.size() - i) / 2);
		}
	}

	// No repeating pattern is found
	return std::make_pair(-1, -1);
}


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

	// Keeps doing cycles until a pattern is found
	int sum = 0;
	std::vector<std::pair<int, int>> dirs = { { -1, 0 }, { 0, -1 }, { 1, 0 }, { 0, 1 } };
	std::vector<int> sums; // Represented as a string because taking substrings is easier
	for (size_t i = 0; i < 1000000000; ++i) {
		// Tilts the map in all four directions, going through one cycle
		for (const auto& dir : dirs) {
			tiltMap(map, dir);
		}

		// Calculates the total weight
		int currentSum = 0;
		for (size_t i = 0; i < map.size(); ++i) {
			currentSum += std::count(map.at(i).begin(), map.at(i).end(), 'O') * (map.size() - i);
		}

		// Looks for a repeating pattern
		sums.push_back(currentSum);
		auto [patternStart, patternLength] = isPatternRepeating(sums);
		if (patternStart != -1) {
			sum = sums.at(patternStart + (1000000000 - patternStart) % patternLength - 1);
			break;
		}
	}

	std::cout << sum << std::endl;
	
	return 0;
}
