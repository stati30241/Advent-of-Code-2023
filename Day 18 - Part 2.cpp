#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <queue>
#include <unordered_set>


// Simple struct to store a position
struct Pos {
	int64_t x, y;
};

bool operator== (const Pos& left, const Pos& right) {
	return left.x == right.x && left.y == right.y;
}

template <>
struct std::hash<Pos> {
	std::size_t operator() (const Pos& pos) const {
		return std::hash<int64_t>() (pos.x) ^ std::hash<int64_t>() (pos.y);
	}
};


int main() {
	// Opens the file
	std::ifstream ifs{ "input.txt" };
	if (!ifs.is_open()) return -1;

	// We will store the positions of the boundary in a set
	// We will also keep track of the topleft and bottomright
	std::vector<Pos> boundaryVertecies;
	Pos currentPos = { 0, 0 };
	int64_t numBoundaryPoints = 0;

	// Reads in the file
	std::string color;
	char dir;
	int amount;
	while (ifs >> dir >> amount >> color) {
		// Extracts the direction and amount from the "color"
		dir = color.at(7);
		std::stringstream colorStream;
		colorStream << std::hex << color.substr(2, color.size() - 4);
		colorStream >> amount;

		// Calculates the normalized vectors for each of the directions
		Pos dirVector = { 0, 0 };
		switch (dir) {
		case '0':
			dirVector = { 1, 0 };
			break;

		case '1':
			dirVector = { 0, 1 };
			break;

		case '2':
			dirVector = { -1, 0 };
			break;

		case '3':
			dirVector = { 0, -1 };
			break;
		}

		// Adds each of the tiles traversed to the boundary
		numBoundaryPoints += amount;
		currentPos.x += amount * dirVector.x;
		currentPos.y += amount * dirVector.y;
		boundaryVertecies.push_back(currentPos);
	}

	// Uses the shoelace theorem to find the interior points
	int64_t a = boundaryVertecies.back().x * boundaryVertecies.front().y;
	int64_t b = boundaryVertecies.front().x * boundaryVertecies.back().y;
	for (size_t i = 0; i < boundaryVertecies.size() - 1; ++i) {
		a += boundaryVertecies.at(i).x * boundaryVertecies.at(i + 1).y;
		b += boundaryVertecies.at(i + 1).x * boundaryVertecies.at(i).y;
	}
	int64_t interiorPoints = std::abs(a - b) / 2;

	// Uses pick's theorem to find the total area
	int64_t area = interiorPoints + numBoundaryPoints / 2 + 1;

	std::cout << area << std::endl;

	return 0;
}
