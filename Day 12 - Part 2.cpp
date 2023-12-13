#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>


// Simple position struct that can be hashed
struct Pos {
	size_t mapIndex, runIndex, groupIndex;
};

bool operator== (const Pos& left, const Pos& right) {
	return left.mapIndex == right.mapIndex &&
		left.runIndex == right.runIndex &&
		left.groupIndex == right.groupIndex;
}

template <>
struct std::hash<Pos> {
	std::size_t operator() (const Pos& pos) const {
		return std::hash<size_t>()(pos.mapIndex) ^
			std::hash<size_t>()(pos.runIndex) ^
			std::hash<size_t>()(pos.groupIndex);
	}
};


// Calculates all the valid possibilites given the map and the groups
uint64_t calculatePossibilities(const std::string& map, const std::vector<int>& groups,
	size_t mapIndex, size_t runIndex, size_t groupIndex, std::unordered_map<Pos, uint64_t>& cache) {

	// If it already exists in cache, just use that value
	if (cache.contains(Pos{ mapIndex, runIndex, groupIndex })) return cache.at({ mapIndex, runIndex, groupIndex });
	
	// If we have reached the end of the map, check if it is a valid arrangement and return that
	if (mapIndex == map.size()) {
		bool isValid = (runIndex == 0 && groupIndex == groups.size()) ||
			(groupIndex == groups.size() - 1 && runIndex == groups.at(groupIndex));
		//cache.clear();
		return isValid;
	}

	uint64_t possibilities = 0;

	// Checks all the possibilites where it is damaged first
	if (map.at(mapIndex) == '#' || map.at(mapIndex) == '?') {
		possibilities += calculatePossibilities(map, groups, mapIndex + 1, runIndex + 1, groupIndex, cache);
	}

	// Checks all the possibilies where is it operational next
	if (map.at(mapIndex) == '.' || map.at(mapIndex) == '?') {
		// If the previous spring was also operational, we haven't really completed a group
		if (runIndex == 0) possibilities += calculatePossibilities(map, groups, mapIndex + 1, 0, groupIndex, cache);
		
		// Now we check if the group we completed was valid or not
		else if (groupIndex < groups.size() && runIndex == groups.at(groupIndex)) {
			possibilities += calculatePossibilities(map, groups, mapIndex + 1, 0, groupIndex + 1, cache);
		}
	}

	// Caches the results
	cache.insert(std::make_pair(Pos{mapIndex, runIndex, groupIndex}, possibilities));

	return possibilities;
}


int main() {
	// Opens the file
	std::ifstream ifs{ "input.txt" };
	if (!ifs.is_open()) return -1;

	// Reads in the file line by line and passes the line
	// into the recursive function
	uint64_t sum = 0;
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

		// Extends the map and the groups
		map = map + "?" + map + "?" + map + "?" + map + "?" + map;
		std::vector<int> expandedGroups;
		for (size_t i = 0; i < 5; ++i) {
			for (int g : groups) {
				expandedGroups.push_back(g);
			}
		}

		std::unordered_map<Pos, uint64_t> cache{ };
		sum += calculatePossibilities(map, expandedGroups, 0, 0, 0, cache);
	}

	std::cout << sum << std::endl;

	return 0;
}
