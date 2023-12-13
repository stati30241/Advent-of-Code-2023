#include <fstream>
#include <iostream>
#include <string>
#include <vector>


bool isSymmetric(const std::vector<std::string>& map, size_t reflectionLine) {
	size_t offset = (reflectionLine <= map.size() / 2) ? reflectionLine : map.size() - reflectionLine;
	size_t start = reflectionLine - offset;
	for (int i = 0; i < offset * 2; ++i) {
		size_t pos = offset * 2 + start - i - 1;
		if (map.at(i + start) != map.at(pos)) return false;
	}
	return true;
}


int main() {
	// Opens the file
	std::ifstream ifs{ "input.txt" };
	if (!ifs.is_open()) return -1;

	// Reads in the maps in two different ways
	std::vector<std::vector<std::string>> maps;
	std::string line = "";
	std::vector<std::string> mapLine;
	while (std::getline(ifs, line)) {
		if (line.empty()) {
			maps.push_back(mapLine);
			mapLine.clear();
			continue;
		}

		mapLine.push_back(line);
	}
	maps.push_back(mapLine);

	// Checks along both vertical and horizontal axis
	int sum = 0;
	for (const auto& map : maps) {
		// Constructs a map that is rotated 90 degrees
		std::vector<std::string> rotatedMap;
		for (size_t j = 0; j < map.at(0).size(); ++j) {
			std::string line = "";
			for (size_t i = 0; i < map.size(); ++i) {
				line += map.at(i).at(j);
			}
			rotatedMap.push_back(line);
		}

		for (size_t reflectionLine = 1; reflectionLine < rotatedMap.size(); ++reflectionLine) {
			if (isSymmetric(rotatedMap, reflectionLine)) sum += reflectionLine;
		}
		for (size_t reflectionLine = 1; reflectionLine < map.size(); ++reflectionLine) {
			if (isSymmetric(map, reflectionLine)) sum += 100 * reflectionLine;
		}
	}

	std::cout << sum << std::endl;

	return 0;
}
