#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>


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

	// Calculates how long it takes to get the last node
	std::string currentNode = "AAA";
	size_t index;
	for (index = 0;; ++index) {
		if (currentNode == "ZZZ") break;
		auto nodeElements = nodes.at(currentNode);
		currentNode = (directions.at(index % directions.size()) == 'L') ? nodeElements.first : nodeElements.second;
	}

	std::cout << index << std::endl;

	return 0;
}
