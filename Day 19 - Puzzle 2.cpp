#include <fstream>
#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>


// Simple struct to store characteristics of a range
struct Range {
	int start, end;
};


// Simple struct to sore a part and its values
struct Part {
	std::unordered_map<char, Range> map{
		{'x', {0, 4000}}, {'m', {0, 4000}}, {'a', {0, 4000}}, {'s', {0, 4000}}
	};

	int64_t calculateCombinations() {
		int64_t product = 1;
		for (const auto& [key, range] : map) {
			product *= range.end - range.start;
		}
		return product;
	}
};


// Simple struct to store comparisions
struct Comparison {
	// Stores the values {catogory<value: result}
	char category = 0;
	char comparison = 0;
	int value = 0;
	std::string result = "";

	Part operator() (Part part, std::vector<Comparison> comps, size_t index) const {
		for (int i = 0; i < index; ++i) {
			Comparison prevComp = comps.at(i);
			prevComp.comparison = (prevComp.comparison == '>') ? '[' : ']'; // Less than or equal to and greater than or equal to
			part = prevComp(part, comps, i);
		}

		if (category == 'd') return part;

		Range& range = part.map.at(category);
		if (range.start > value || range.end < value) return part;

		switch (comparison) {
		case '<':
			range.end = value - 1;
			break;
		case '>':
			range.start = value;
			break;
		case '[':
			range.end = value;
			break;
		case ']':
			range.start = value - 1;
			break;
		}
		return part;
	}
};


// Stores the properties of a workflow
struct Workflow {
	std::vector<Comparison> comparisons;
	Part part;
};


int main() {
	// Opens the file
	std::ifstream ifs{ "input.txt" };
	if (!ifs.is_open()) return -1;

	// Reads in the workflow
	std::unordered_map<std::string, Workflow> workflows;
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

		workflows.insert({ key, {comparisons} });
	}

	// Calculates all the combinations by traversing the graph of all the workflows
	int64_t sum = 0;
	std::queue<Workflow> currentWorkflows;
	currentWorkflows.push(workflows.at("in"));
	while (!currentWorkflows.empty()) {
		Workflow current = currentWorkflows.front();
		currentWorkflows.pop();

		// Goes through all the comparisons and splits ranges
		for (size_t i = 0; i < current.comparisons.size(); ++i) {
			Comparison comp = current.comparisons.at(i);
			if (comp.result == "A") {
				// If accepted, add the combinations of this range to the sum
				sum += comp(current.part, current.comparisons, i).calculateCombinations();
				continue;
			} else if (comp.result == "R") {
				continue;
			}

			// Creates a new workflow to add to the queue
			Workflow next = workflows.at(comp.result);
			next.part = comp(current.part, current.comparisons, i);
			currentWorkflows.push(next);
		}
	}

	std::cout << sum << std::endl;

	return 0;
}
