#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>


// Simple struct to sore a part and its values
struct Part {
	std::unordered_map<char, int> map{
		{'x', 0}, {'m', 0}, {'a', 0}, {'s', 0}
	};
};


// Simple struct to store comparisions
struct Comparison {
	// Stores the values {catogory<value: result}
	char category = 0;
	char comparison = 0;
	int value = 0;
	std::string result = "";

	bool operator() (const Part& part) const {
		if (category == 'd') return true;

		if (comparison == '<') {
			return part.map.at(category) < value;
		} else if (comparison == '>') {
			return part.map.at(category) > value;
		}
	}
};


int main() {
	// Opens the file
	std::ifstream ifs{ "input.txt" };
	if (!ifs.is_open()) return -1;

	// Reads in the workflow
	std::unordered_map<std::string, std::vector<Comparison>> workflows;
	for (std::string line = ""; std::getline(ifs, line) && !line.empty();) {
		// Parses each line
		std::string token = "";
		std::string key = "";
		std::vector<Comparison> comparisons;
		Comparison comp;
		for (char ch : line) {
			switch (ch) {
			case '{':
				key = token;
				token = "";
				break;

			case '>':
			case '<':
				comp.category = token.front();
				comp.comparison = ch;
				token = "";
				break;

			case ':':
				comp.value = std::stoi(token);
				token = "";
				break;

			case ',':
				comp.result = token;
				comparisons.push_back(comp);
				comp = Comparison{};
				token = "";
				break;

			case '}':
				comp.category = 'd';
				comp.result = token;
				comparisons.push_back(comp);
				comp = Comparison{};
				token = "";
				break;

			default:
				token += ch;
			}
		}

		workflows.insert({ key, comparisons });
	}

	// Reads in the ratings of the parts and calculates whether they get accepted or not
	int sum = 0;
	for (std::string line = ""; std::getline(ifs, line);) {
		// Reads in the part
		Part part;
		std::string token = "";
		char label;
		for (char ch : line) {
			if (isalpha(ch)) label = ch;
			if (isdigit(ch)) token += ch;
			else if (ch == ',' || ch == '}') {
				part.map.at(label) = std::stoi(token);
				label = 0;
				token = "";
			}
		}

		// Determines if the part is qualified or not
		std::string currentWorkflow = "in";
		while (currentWorkflow != "A" && currentWorkflow != "R") {
			std::vector<Comparison> workflow = workflows.at(currentWorkflow);
			for (const auto& comparison : workflow) {
				if (!comparison(part)) continue;
				currentWorkflow = comparison.result;
				break;
			}
		}

		// If the part gets accepted, add the part's ratings to the sum
		if (currentWorkflow == "A") {
			sum += part.map.at('x') + part.map.at('m') + part.map.at('a') + part.map.at('s');
		}
	}

	std::cout << sum << std::endl;

	return 0;
}
