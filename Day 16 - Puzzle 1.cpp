#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include <unordered_set>


// Simple struct, stores a 2d vector
struct Vec2d {
	int x, y;
};

Vec2d operator+ (const Vec2d& left, const Vec2d& right) {
	return Vec2d{ left.x + right.x, left.y + right.y };
}


// Struct to simulate a beam of light
struct Light {
	Vec2d pos;
	Vec2d dir;
};

bool operator== (const Light& left, const Light& right) {
	return left.pos.x == right.pos.x && left.pos.y == right.pos.y &&
		left.dir.x == right.dir.x && left.dir.y == right.dir.y;
}

template <>
struct std::hash<Light> {
	bool operator() (const Light& beam) const {
		return std::hash<int>() (beam.pos.x) ^ std::hash<int>() (beam.pos.y)
			^ std::hash<int>() (beam.dir.x) ^ std::hash<int>() (beam.dir.y);
	}
};


int main() {
	// Opens the file
	std::ifstream ifs{ "input.txt" };
	if (!ifs.is_open()) return -1;

	// Reads in the map
	std::vector<std::string> map;
	std::string line = "";
	while (ifs >> line) map.push_back(line);

	// Simulates the beam of light going into the contraption
	std::vector<Light> beamsOfLight{ { {0, 0}, {1, 0} } };
	std::vector<Light> beamsOfLightCopy;
	std::unordered_set<Light> visitedTiles;

	while (!beamsOfLight.empty()) {
		// Clears the copy of beams of light
		beamsOfLightCopy.clear();

		for (const auto& beam : beamsOfLight) {
			// If the beam escapes the map, flag it for deletion
			if (beam.pos.x < 0 || beam.pos.x >= map.at(0).size() ||
				beam.pos.y < 0 || beam.pos.y >= map.size()) {
				continue;
			}

			// If we have already visited the tile before with the same
			// direction, then the beam is going in a loop, and we can
			// get rid of it
			if (visitedTiles.contains(beam)) {
				continue;
			}

			// If it's still in the map, add it to the copy of beams
			// and record the tile as energized
			beamsOfLightCopy.push_back(beam);
			Light& newBeam = beamsOfLightCopy.back();
			visitedTiles.insert(beam);

			// Determines what to do depending on what tile the beam is on
			switch (map.at(beam.pos.y).at(beam.pos.x)) {
			case '/':
				newBeam.dir = { -beam.dir.y, -beam.dir.x };
				break;

			case '\\':
				newBeam.dir = { +beam.dir.y, +beam.dir.x };
				break;

			case '|':
				if (beam.dir.x) {
					newBeam.dir = { 0, -1 };
					beamsOfLightCopy.push_back({ beam.pos, { 0, 1 } });
				}
				break;

			case '-':
				if (beam.dir.y) {
					newBeam.dir = { -1, 0 };
					beamsOfLightCopy.push_back({ beam.pos, { 1, 0 } });
				}
				break;

			case '.':
				break;
			}
		}

		// Updates the list of beams
		beamsOfLight = beamsOfLightCopy;
		for (auto& beam : beamsOfLight) {
			beam.pos = beam.pos + beam.dir;
		}
	}

	// Calculates the total number of tiles that have been energized
	int totalEnergizedTiles = 0;
	for (int j = 0; j < map.size(); ++j) {
		for (int i = 0; i < map.at(j).size(); ++i) {
			auto search = std::find_if(visitedTiles.begin(), visitedTiles.end(),
				[&](const Light& light) { return i == light.pos.x && j == light.pos.y; });
			if (search != visitedTiles.end()) totalEnergizedTiles++;
		}
	}
	
	std::cout << totalEnergizedTiles << std::endl;

	return 0;
}
