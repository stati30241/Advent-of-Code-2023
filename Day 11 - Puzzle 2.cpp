#include <fstream>
#include <iostream>
#include <string>
#include <vector>


int main() {
	// Opens the file
	std::ifstream ifs{ "input.txt" };
	if (!ifs.is_open()) return -1;

	// Reads in the map
	std::vector<std::string> originalMap;
	std::string line = "";
	while (std::getline(ifs, line)) originalMap.push_back(line);

	// Accounts for the expansion of the universe
	std::vector<std::string> map;
	for (size_t i = 0; i < originalMap.size(); ++i) {
		bool containsGalaxy = false;
		for (size_t j = 0; j < originalMap.at(i).size(); ++j) {
			if (originalMap.at(i).at(j) == '#') containsGalaxy = true;
		}
		if (containsGalaxy) map.push_back(originalMap.at(i));
		else map.insert(map.cend(), 2, originalMap.at(i));
	}
	size_t blankLines = 0;
	for (size_t j = 0; j < originalMap.at(0).size(); ++j) {
		bool containsGalaxy = false;
		for (size_t i = 0; i < originalMap.size(); ++i) {
			if (originalMap.at(i).at(j) == '#') containsGalaxy = true;
		}
		if (!containsGalaxy) {
			for (std::string& line : map) {
				line.insert(j + blankLines, 1, '.');
			}
			blankLines++;
		}
	}

	// Stores the positions of the galaxies
	std::vector<std::pair<int, int>> galaxies;
	for (size_t i = 0; i < map.size(); ++i) {
		for (size_t j = 0; j < map.at(i).size(); ++j) {
			if (map.at(i).at(j) == '#') galaxies.emplace_back(i, j);
		}
	}

	// Calculates the distances between each pair of galaxies
	int totalDistance = 0;
	for (size_t i = 0; i < galaxies.size(); ++i) {
		for (size_t j = i + 1; j < galaxies.size(); ++j) {
			int distance = std::abs(galaxies.at(j).first - galaxies.at(i).first)
				+ std::abs(galaxies.at(j).second - galaxies.at(i).second);
			totalDistance += distance;
		}
	}

	std::cout << totalDistance << std::endl;

	return 0;
}
