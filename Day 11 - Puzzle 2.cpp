#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>


int main() {
	// Opens the file
	std::ifstream ifs{ "input.txt" };
	if (!ifs.is_open()) return -1;

	// Reads in the map
	std::vector<std::string> map;
	std::string line = "";
	while (std::getline(ifs, line)) map.push_back(line);

	// Accounts for the expansion of the universe and stores the
	// positions of the galaxies
	std::vector<std::pair<int, int>> galaxies;
	std::set<size_t> emptyRows, emptyColumns;
	for (size_t i = 0; i < map.size(); ++i) {
		bool containsGalaxy = false;
		for (size_t j = 0; j < map.at(i).size(); ++j) {
			if (map.at(i).at(j) == '#') {
				containsGalaxy = true;
				galaxies.emplace_back(i, j);
			}
		}
		if (!containsGalaxy) {
			emptyRows.insert(i);
		}
	}
	for (size_t j = 0; j < map.at(0).size(); ++j) {
		bool containsGalaxy = false;
		for (size_t i = 0; i < map.size(); ++i) {
			if (map.at(i).at(j) == '#') containsGalaxy = true;
		}
		if (!containsGalaxy) {
			emptyColumns.insert(j);
		}
	}

	// Calculates the distances between each pair of galaxies
	uint64_t expansionFactor = 1'000'000;
	uint64_t totalDistance = 0;
	for (size_t i = 0; i < galaxies.size(); ++i) {
		for (size_t j = i + 1; j < galaxies.size(); ++j) {
			// Calculates the normal distance between galaxies
			int distance = std::abs(galaxies.at(j).first - galaxies.at(i).first)
				+ std::abs(galaxies.at(j).second - galaxies.at(i).second);
			
			// Accounts for the expansion of the universe
			size_t start = std::min(galaxies.at(i).first, galaxies.at(j).first);
			size_t end = std::max(galaxies.at(i).first, galaxies.at(j).first);
			for (size_t k = start; k <= end; ++k) {
				if (emptyRows.contains(k)) distance += expansionFactor - 1;
			}
			start = std::min(galaxies.at(i).second, galaxies.at(j).second);
			end = std::max(galaxies.at(i).second, galaxies.at(j).second);
			for (size_t k = start; k <= end; ++k) {
				if (emptyColumns.contains(k)) distance += expansionFactor - 1;
			}

			totalDistance += distance;
		}
	}

	std::cout << totalDistance << std::endl;

	return 0;
}
