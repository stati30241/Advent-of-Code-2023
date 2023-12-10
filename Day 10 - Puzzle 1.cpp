#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>


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

	// Reads the map into a 2d array and adds padding
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

	// Finds the starting points to start searching
	Pos forwardTraverser{ }, backwardTraverser{ };
	std::vector<Pos> neighbors = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };
	for (const Pos& neighbor : neighbors) {
		char ch = map(start + neighbor);
		if (ch == '.') continue;
		const auto pipeDirections = pipes.at(ch);
		if (start == start + neighbor + pipeDirections.first ||
			start == start + neighbor + pipeDirections.second) {
			if (forwardTraverser == Pos{ }) forwardTraverser = start + neighbor;
			else backwardTraverser = start + neighbor;
		}
	}

	// Traverses the loop to find the farthest point
	// At the farthest point, the forward and backward traversal person would have met up
	// (Yes I am imagining two people walking from opposite sides trying to meet each other)
	int steps = 1;
	Pos prevForwardTraverser = start, prevBackwardTraverser = start;
	while (forwardTraverser != backwardTraverser) {
		// The person traversing forwards 
		const auto forwardPipeDirections = pipes.at(map(forwardTraverser));
		if (forwardPipeDirections.first + forwardTraverser == prevForwardTraverser) {
			prevForwardTraverser = forwardTraverser;
			forwardTraverser = forwardPipeDirections.second + forwardTraverser;
		} else {
			prevForwardTraverser = forwardTraverser;
			forwardTraverser = forwardPipeDirections.first + forwardTraverser;
		}

		// The person traversing backwards
		const auto backwardPipeDirections = pipes.at(map(backwardTraverser));
		if (backwardPipeDirections.first + backwardTraverser == prevBackwardTraverser) {
			prevBackwardTraverser = backwardTraverser;
			backwardTraverser = backwardPipeDirections.second + backwardTraverser;
		} else {
			prevBackwardTraverser = backwardTraverser;
			backwardTraverser = backwardPipeDirections.first + backwardTraverser;
		}

		// Increments the steps
		steps++;
	}

	std::cout << steps << std::endl;

	return 0;
}
