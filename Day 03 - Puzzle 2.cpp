#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>


int main() {
	// Opens the file
	std::ifstream ifs{ "input.txt" };
	if (!ifs.is_open()) return -1;

	// Puts the entire input into a 2d array with padding
	std::vector<std::string> schematic;
	std::string line = "";
	while (std::getline(ifs, line)) {
		line = "." + line + "."; // Padding
		schematic.push_back(line);
	}
	// Adds the top and bottom padding
	schematic.emplace(schematic.begin(), std::string(schematic.at(1).size(), '.'));
	schematic.push_back(schematic.at(0));
	
	// The sum that is to be calculated
	int sum = 0;
	// The neighbors of any specific point
	std::vector<std::pair<size_t, size_t>> neighbors = {
		{ -1, -1 }, { +0, -1 }, { +1, -1 },
		{ -1, +0 },             { +1, +0 },
		{ -1, +1 }, { +0, +1 }, { +1, +1 },
	};
	std::map<std::pair<int, int>, int> gears;

	// Goes through our 2d grid
	std::pair<int, int> gearPos = std::make_pair(0, 0);
	std::string token = "";
	for (size_t i = 0; i < schematic.size(); ++i) {
		for (size_t j = 0; j < schematic.at(0).size(); ++j) {
			// If the character is not a number, we don't care about it
			char ch = schematic.at(i).at(j);
			if (ch < 48 || ch > 57) {
				if (!token.empty() && (gearPos.first != 0 && gearPos.second != 0)) {
					// If the gear already has a label, then we multiply those both, 
					// otherwise we note down this gear's position and the label it has
					if (gears.contains(gearPos)) sum += gears.at(gearPos) * std::atoi(token.c_str());
					else gears.insert(std::make_pair(gearPos, std::atoi(token.c_str())));
				}
				token = "";
				gearPos = std::make_pair(0, 0);
				continue;
			}
			
			// Add the character to the token if it is a number
			token += ch;

			// Checks the neighbors for a machine part
			for (const auto& neighborCoord : neighbors) {
				// It is a part if it is not a number or a dot
				char neighbor = schematic.at(i + neighborCoord.first).at(j + neighborCoord.second);
				if (neighbor == '*') gearPos = std::make_pair(i + neighborCoord.first, j + neighborCoord.second);
			}
		}
	}
	
	// Prints out the sum
	std::cout << sum << std::endl;

	return 0;
}
