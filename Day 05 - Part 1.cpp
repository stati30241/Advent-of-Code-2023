#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


// Simple struct to store the mappings
struct Mapping {
	int64_t destinationRangeStart;
	int64_t sourceRangeStart;
	int64_t rangeLength;
};


int main() {
	// Opens the file
	std::ifstream ifs{ "input.txt" };
	if (!ifs.is_open()) return -1;

	// Reads in the seeds (i hate parsing)
	std::vector<int64_t> seeds;
	std::string seedLine = "";
	std::getline(ifs, seedLine);
	std::stringstream seedLineStream{ seedLine };
	std::string discard = "";
	int64_t currentSeed = 0;
	seedLineStream >> discard;
	while (seedLineStream >> currentSeed) {
		seeds.push_back(currentSeed);
	}

	// Reads in the ranges and calculates the stores them
	// The ranges are stored in sections so they can evaluated in bunches
	std::string line = "";
	std::vector<std::vector<Mapping>> mappings;
	while (std::getline(ifs, line)) {
		if (line.empty()) mappings.push_back({});
		if (line.empty() || !isdigit(line.front())) continue;

		std::stringstream lineStream{ line };
		Mapping mapping;
		lineStream >> mapping.destinationRangeStart >> mapping.sourceRangeStart >> mapping.rangeLength;
		mappings.back().push_back(mapping);
	}

	// Calculates the mappings of each of the seeds
	// (God I hate nested loops but it is what it is)
	for (int64_t& seed : seeds) {
		for (const auto& section : mappings) {
			for (const auto& mapping : section) {
				if (seed > mapping.sourceRangeStart && seed <= mapping.sourceRangeStart + mapping.rangeLength) {
					seed += mapping.destinationRangeStart - mapping.sourceRangeStart;
					break;
				}
			}
		}
	}

	// Finds the minimum location number
	int64_t minLocationNumber = std::numeric_limits<int64_t>::max();
	for (int64_t seed : seeds) {
		if (seed < minLocationNumber) minLocationNumber = seed;
	}

	std::cout << minLocationNumber << std::endl;

	return 0;
}
