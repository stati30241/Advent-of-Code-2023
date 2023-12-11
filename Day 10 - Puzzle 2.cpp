#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>


// Simple struction to store position data
struct Pos {
	int x, y;
};

bool operator== (const Pos& left, const Pos& right) {
	return left.x == right.x && left.y == right.y;
}
Pos operator+ (const Pos& left, const Pos& right) {
	return Pos{ left.x + right.x, left.y + right.y };
}
Pos operator- (const Pos& left, const Pos& right) {
	return Pos{ left.x - right.x, left.y - right.y };
}


// Custom hash function defined for the Pos
template <>
struct std::hash<Pos> {
	std::size_t operator() (const Pos& pos) const {
		return std::hash<int>()(pos.x) ^ std::hash<int>()(pos.y);
	}
};


// Makes the manipulation of the 2d map easier
struct Map {
	std::vector<std::string> map;

	size_t width() const { return map.at(0).size(); }
	size_t height() const { return map.size(); }

	char& operator() (const Pos& pos) { return map.at(pos.y).at(pos.x); }
	char operator() (const Pos& pos) const { return map.at(pos.y).at(pos.x); }
	char& operator() (size_t i, size_t j) { return map.at(j).at(i); }
	char operator() (size_t i, size_t j) const { return map.at(j).at(i); }
};


int main() {
	// Opens the file
	std::ifstream ifs{ "input.txt" };
	if (!ifs.is_open()) return -1;

	// Reads the map into a 2d array
	Map map;
	std::string line = "";
	while (std::getline(ifs, line)) map.map.push_back("." + line + ".");
	map.map.emplace(map.map.begin(), std::string(map.map.at(1).size(), '.'));
	map.map.push_back(std::string(map.map.at(1).size(), '.'));

	// Figures out a starting position
	Pos start{};
	for (int i = 0; i < map.width(); ++i) {
		for (int j = 0; j < map.height(); ++j) {
			if (map(i, j) == 'S') {
				start = { i, j };
				break;
			}
		}
	}

	// A hashmap containing all the positions that the different
	// pipes can lead to
	std::unordered_map<char, std::pair<Pos, Pos>> pipes = {
		{'|', { {0, -1}, {0, +1} }},
		{'-', { {-1, 0}, {+1, 0} }},
		{'L', { {0, -1}, {+1, 0} }},
		{'J', { {0, -1}, {-1, 0} }},
		{'7', { {0, +1}, {-1, 0} }},
		{'F', { {0, +1}, {+1, 0} }}
	};

	// Finds the next point to start traversing the loop
	Pos traverser{ };
	std::vector<Pos> neighbors = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };
	std::vector<Pos> startNeighbors; // We are gonna use to determine what pipe start is
	for (const Pos& neighbor : neighbors) {
		char ch = map(start + neighbor);
		if (ch == '.') continue;
		const auto pipeDirections = pipes.at(ch);
		if (start == start + neighbor + pipeDirections.first ||
			start == start + neighbor + pipeDirections.second) {
			startNeighbors.push_back(neighbor);
			traverser = start + neighbor;
		}
	}

	// Figures out which pipe start is and replaces S with that pipe
	for (const auto& [pipe, pipeNeighbors] : pipes) {
		if ((pipeNeighbors.first == startNeighbors.at(0) &&
			pipeNeighbors.second == startNeighbors.at(1)) ||
			(pipeNeighbors.first == startNeighbors.at(1) &&
				pipeNeighbors.second == startNeighbors.at(0))) {
			map(start) = pipe;
		}
	}

	// Traverses around the loop and adds all the positions to a set for easy lookup
	std::unordered_set<Pos> loop{ start };
	Pos prevTraverser = start;
	while (traverser != start) {
		// Adds the traversed position to the loop set
		loop.insert(traverser);

		// The person traversing forwards 
		const auto pipeDirections = pipes.at(map(traverser));
		if (pipeDirections.first + traverser == prevTraverser) {
			prevTraverser = traverser;
			traverser = pipeDirections.second + traverser;
		} else {
			prevTraverser = traverser;
			traverser = pipeDirections.first + traverser;
		}
	}

	// Calculates how many tiles are enclosed by the loop by
	// casting a ray from the point to the end of map
	int tilesEnclosed = 0;
	for (int i = 0; i < map.width(); ++i) {
		for (int j = 0; j < map.height(); ++j) {
			// Disregards the point if it lies on the loop
			if (loop.contains({ i, j })) continue;

			// Calculates how many times the ray collides with the loop
			size_t numCollisions = 0;
			for (int p = i; p > 0; --p) {
				if (loop.contains({ p, j }) && // Have to be careful here, not all collisions with the loop shoudl count
					map(p, j) != '-' && map(p, j) != 'F' && map(p, j) != '7') numCollisions++;
			}

			// If the number of collisions is even, then its outside
			if (numCollisions % 2 == 1) {
				tilesEnclosed++;
			}
		}
	}

	std::cout << tilesEnclosed << std::endl;

	return 0;
}
