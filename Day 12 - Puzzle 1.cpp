#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


// Calculates all the valid possibilites given the map and the groups
int calculatePossibilities(const std::string& map, std::vector<int> groups) {
	int possibilities = 0;

	// There are 2^n possibilities total with n being the number of question marks
	size_t count = std::pow(2, std::count(map.begin(), map.end(), '?'));
	for (size_t i = 0; i < count; ++i) {
		// Calculates a new possibility
		// I'm sure there is a better way to do this but I can't seem
		// to program today so oh well
		std::string newMap = map;
		size_t j = 1;
		for (char& ch : newMap) {
			if (ch == '.') ch = ' ';
			if (ch != '?') continue;
			ch = ((i & j) == j) ? '#' : ' ';
			j *= 2;
		}
		
		// Checks if the possibility is valid
		std::stringstream newMapStream{ newMap };
		bool yes = true;
		std::string token = "";
		size_t k = 0;
		for (k = 0; newMapStream >> token; ++k) {
			if (k >= groups.size()) {
				yes = false;
				break;
			}
			if (token.size() != groups.at(k)) {
				yes = false;
				break;
			}
		}
		if (yes && k == groups.size()) {
			possibilities++;
		}
	}

	return possibilities;
}


int main() {
	// Opens the file
	std::ifstream ifs{ "input.txt" };
	if (!ifs.is_open()) return -1;

	// Reads in the file line by line and passes the line
	// into the recursive function
	int sum = 0;
	std::string map = "", groupString = "";
	while (ifs >> map >> groupString) {
		// Reads in the groups (yes this is very unelegant)
		std::replace(groupString.begin(), groupString.end(), ',', ' ');
		std::stringstream groupsStream{ groupString };
		std::vector<int> groups;
		int group = 0;
		while (groupsStream >> group) {
			groups.push_back(group);
		}

		sum += calculatePossibilities(map, groups);
	}

	std::cout << sum << std::endl;

	return 0;
}
