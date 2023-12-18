#include <fstream>
#include <iostream>
#include <queue>
#include <unordered_set>


// Simple struct to store a position
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


int main() {
	// Opens the file
	std::ifstream ifs{ "input.txt" };
	if (!ifs.is_open()) return -1;

	// We will store the positions of the boundary in a set
	// We will also keep track of the topleft and bottomright
	std::unordered_set<Pos> boundary;
	Pos currentPos = { 0, 0 };
	int left = std::numeric_limits<int>::max(), top = std::numeric_limits<int>::max();
	int right = std::numeric_limits<int>::min(), bottom = std::numeric_limits<int>::min();

	// Reads in the file
	std::string color;
	char dir;
	int amount;
	while (ifs >> dir >> amount >> color) {
		// Calculates the normalized vectors for each of the directions
		Pos dirVector;
		switch (dir) {
		case 'U':
			dirVector = { 0, -1 };
			break;

		case 'L':
			dirVector = { -1, 0 };
			break;

		case 'D':
			dirVector = { 0, 1 };
			break;

		case 'R':
			dirVector = { 1, 0 };
			break;
		}

		// Adds each of the tiles traversed to the boundary
		for (size_t i = 0; i < amount; ++i) {
			currentPos.x += dirVector.x;
			currentPos.y += dirVector.y;
			boundary.insert(currentPos);

			if (currentPos.x < left) left = currentPos.x;
			if (currentPos.y < top) top = currentPos.y;
			if (currentPos.x > right) right = currentPos.x;
			if (currentPos.y > bottom) bottom = currentPos.y;
		}
	}

	// Uses the floodfill algorithm to find the amount of tiles
	// outside the boundary, starts with the point just above topleft
	// so it is guaranteed to be on the outside
	std::queue<Pos> queue;
	std::unordered_set<Pos> outside;
	queue.push(Pos{ left - 1, top - 1 });
	while (!queue.empty()) {
		Pos current = queue.front();
		queue.pop();

		// Skip the tile if we already looked into it, is out of bounds, or on the boundary
		if (boundary.contains(current)) continue;
		if (current.x < left - 1 || current.x > right + 1 ||
			current.y < top - 1 || current.y > bottom + 1) {
			continue;
		}
		if (outside.contains(current)) continue;

		outside.insert(current);

		// Adds neighbors to the queue
		for (const auto& [dx, dy] : std::vector<Pos>{ {-1, 0}, {0, -1}, {1, 0}, {0, 1} }) {
			queue.push({ current.x + dx, current.y + dy });
		}
	}

	// Calculates the number of tiles on the boundary and its inside
	int numTilesInside = (right - left + 3) * (bottom - top + 3) - outside.size();

	std::cout << numTilesInside << std::endl;

	return 0;
}
