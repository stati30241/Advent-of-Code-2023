#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <numeric>


int main() {
	// Opens the file
	std::ifstream ifs{ "input.txt" };
	if (!ifs.is_open()) return -1;

	std::string directions = "";
	std::unordered_map<std::string, std::pair<std::string, std::string>> nodes;

	// Reads in the directions
	char ch;
	while (ifs.get(ch)) {
		if (ch != 'R' && ch != 'L') break;
		directions += ch;
	}

	// Reads in the nodes
	std::string nodeName = "", discard = "", right = "", left = "";
	while (ifs >> nodeName >> discard >> right >> left) {
		right = right.substr(1, 3);
		left = left.substr(0, 3);
		nodes.insert({ nodeName, {right, left} });
	}

	// Gets all the current nodes
	std::vector<std::pair<std::string, int>> currentNodes;
	for (const auto& pair : nodes) {
		if (pair.first.back() == 'A') currentNodes.push_back(std::make_pair(pair.first, 0));
	}

	while (true) {
		bool done = true;
		for (auto& currentNode : currentNodes) {
			if (currentNode.first.back() != 'Z') done = false;
			if (currentNode.first.back() == 'Z') continue;
			auto nodeElements = nodes.at(currentNode.first);
			currentNode.first = (directions.at(currentNode.second % directions.size()) == 'L') ? nodeElements.first : nodeElements.second;
			currentNode.second++;
		}
		if (done) break;
	}

	uint64_t index = 1;
	for (auto& currentNode : currentNodes) {
		index = std::lcm(index, currentNode.second);
	}
	
	std::cout << index << std::endl;

	return 0;
}
