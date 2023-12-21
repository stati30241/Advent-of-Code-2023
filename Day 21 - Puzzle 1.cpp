#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_set>


// Simple struct to store position data
struct Pos {
	int x, y;
};

bool operator== (const Pos& left, const Pos& right) {
	return left.x == right.x && left.y == right.y;
}

template <>
struct std::hash<Pos> {
	std::size_t operator() (const Pos& pos) const {
		return std::hash<int>() (pos.x) ^ std::hash<int>() (pos.y);
	}
};


// Walks one step and returns a vector with all the reachable positions
std::unordered_set<Pos> walkOneStep(const std::vector<std::string>& map, const std::unordered_set<Pos>& current) {
	std::unordered_set<Pos> visitedTiles{ };
	for (const auto& pos : current) {
		for (const auto& [x, y] : std::vector<std::pair<int, int>>{ {-1, 0}, {0, -1}, {1, 0}, {0, 1} }) {
			Pos neighbor = { pos.x + x, pos.y + y };

			if (neighbor.x < 0 || neighbor.x > map.at(0).size() || neighbor.y < 0 || neighbor.y > map.size()) continue;

			if (map.at(neighbor.y).at(neighbor.x) == '#') continue;

			if (visitedTiles.contains(neighbor)) continue;
			visitedTiles.insert(neighbor);
		}
	}
	return visitedTiles;
}


int main() {
	// Opens the file
	std::ifstream ifs{ "input.txt" };
	if (!ifs.is_open()) return -1;

	// Reads in the map and the finds the starting position
	std::vector<std::string> map;
	Pos startPos{ };
	std::string line = "";
	for (int j = 0; std::getline(ifs, line); ++j) {
		map.push_back(line);
		for (int i = 0; i < line.size(); ++i) {
			if (line.at(i) == 'S') startPos = { i, j };
		}
	}

	std::unordered_set<Pos> visitedTiles{ startPos };
	for (size_t steps = 0; steps < 64; ++steps) {
		visitedTiles = walkOneStep(map, visitedTiles);
	}

	std::cout << visitedTiles.size() << std::endl;

	return 0;
}
