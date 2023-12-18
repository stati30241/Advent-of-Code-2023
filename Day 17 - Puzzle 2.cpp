#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_set>


/*
* Today broke me, curse you dijkstra, i hope you burn in hell
*/


// Simple struct to store the values of each tile as a node
struct Node {
	int x, y;
	int heatLoss = std::numeric_limits<int>::max();
	int dx, dy;
	int steps;
};

bool operator> (const Node& left, const Node& right) {
	return left.heatLoss > right.heatLoss;
}

bool operator== (const Node& left, const Node& right) {
	return left.x == right.x && left.y == right.y && left.dx == right.dx && left.dy == right.dy && left.steps == right.steps;
}

template <>
struct std::hash<Node> {
	std::size_t operator() (const Node& node) const {
		return std::hash<int>() (node.x) ^ std::hash<int>() (node.y) ^
			std::hash<int>() (node.dx) ^ std::hash<int>() (node.dy) ^ std::hash<int>() (node.steps);
	}
};


int main() {
	// Opens the file
	std::ifstream ifs{ "input.txt" };
	if (!ifs.is_open()) return -1;

	// Reads in the input
	std::vector<std::vector<int>> map;
	for (std::string str = ""; std::getline(ifs, str);) {
		std::vector<int> line;
		for (char ch : str) {
			line.push_back(ch - 48);
		}
		map.push_back(line);
	}

	// Uses dijkstra's algorithm to find the shortest path
	int minHeatLoss = 0;
	std::unordered_set<Node> visited;
	std::priority_queue<Node, std::vector<Node>, std::greater<Node>> queue;
	queue.push({ 0, 0, map.at(0).at(0), 0, 0, 0 });
	while (!queue.empty()) {
		Node current = queue.top();
		queue.pop();

		if (visited.contains(current)) continue;
		visited.insert(current);

		if (current.x == map.at(0).size() - 1 && current.y == map.size() - 1 && current.steps >= 4) {
			minHeatLoss = current.heatLoss;
			break;
		}

		if (current.steps < 10 && (current.dx != 0 || current.dy != 0)) {
			int x = current.x + current.dx;
			int y = current.y + current.dy;
			if (x >= 0 && x < map.at(0).size() && y >= 0 && y < map.size()) {
				Node next{ x, y, current.heatLoss + map.at(y).at(x), current.dx, current.dy, current.steps + 1 };
				queue.push(next);
			}
		}

		if (current.steps >= 4 || (current.dx == 0 && current.dy == 0)) {
			for (const auto& [dx, dy] : std::vector<std::pair<int, int>>{ { -1, 0 }, { 0, -1 }, { 1, 0 }, { 0, 1 } }) {
				int x = current.x + dx;
				int y = current.y + dy;

				if (dx == current.dx && dy == current.dy) continue;
				if (dx == -current.dx && dy == -current.dy) continue;

				if (x >= 0 && x < map.at(0).size() && y >= 0 && y < map.size()) {
					Node next{ x, y, current.heatLoss + map.at(y).at(x), dx, dy, 1 };
					queue.push(next);
				}
			}
		}
	}

	std::cout << minHeatLoss - map.front().front() << std::endl;

	return 0;
}
