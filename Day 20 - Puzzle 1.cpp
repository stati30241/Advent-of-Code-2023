#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>


// Simple struct to store the type and values of modules
struct Module {
	std::queue<bool> pulses; // High is true, low is false
	char type = '\0';
	std::vector<std::string> connected;
	bool state = false;
	std::unordered_map<std::string, bool> recentPulses;
};


// Presses the button once
std::pair<int, int> pressButton(std::unordered_map<std::string, Module>& modules) {
	// Uses a queue to keep track of what module to execute
	int numLowPulses = 0, numHighPulses = 0;
	std::queue<std::string> moduleQueue;
	moduleQueue.push("broadcaster");
	numLowPulses++;

	// Loops until there are no more pulses being sent
	while (!moduleQueue.empty()) {
		std::string currentName = moduleQueue.front();
		Module& current = modules.at(currentName);
		moduleQueue.pop();

		// Decides what pulses to send based on modules
		switch (current.type) {
		case 'b':
			for (const auto& output : current.connected) {
				numLowPulses++;
				if (!modules.contains(output)) continue;
				Module& next = modules.at(output);
				next.pulses.push(false);
				if (next.type == '&') {
					next.recentPulses.at(currentName) = false;
				}
				moduleQueue.push(output);
			}
			break;
		case '%':
			if (current.pulses.front()) {
				current.pulses.pop();
				break;
			}
			current.state = !current.state;
			current.pulses.pop();

			for (const auto& output : current.connected) {
				if (current.state) numHighPulses++;
				else numLowPulses++;
				if (!modules.contains(output)) continue;
				Module& next = modules.at(output);
				next.pulses.push(current.state);
				if (next.type == '&') {
					next.recentPulses.at(currentName) = current.state;
				}
				moduleQueue.push(output);
			}
			break;
		case '&':
			bool pulseToSend = false;
			for (const auto& [name, input] : current.recentPulses) {
				if (!input) pulseToSend = true;
			}
			current.pulses.pop();

			for (const auto& output : current.connected) {
				if (pulseToSend) numHighPulses++;
				else numLowPulses++;
				if (!modules.contains(output)) continue;
				Module& next = modules.at(output);
				next.pulses.push(pulseToSend);
				if (next.type == '&') {
					next.recentPulses.at(currentName) = pulseToSend;
				}
				moduleQueue.push(output);
			}
			break;
		}
	}

	return std::make_pair(numLowPulses, numHighPulses);
}


int main() {
	// Opens the file
	std::ifstream ifs{ "input.txt" };
	if (!ifs.is_open()) return -1;

	// Parses the input line by line
	std::unordered_map<std::string, Module> modules;
	for (std::string str = ""; std::getline(ifs, str);) {
		// Makes the string easier to parse through by getting rid of random characters
		std::replace_if(str.begin(), str.end(), 
			[](char ch) { return !isalpha(ch) && ch != '%' && ch != '&'; }, ' ');
		std::stringstream line{ str };

		// Adds the module based on that line 
		Module lineModule;
		std::string name;
		line >> name;
		lineModule.type = name.at(0);
		for (std::string connectedModule = ""; line >> connectedModule;) {
			lineModule.connected.push_back(connectedModule);
		}
		if (name.at(0) == '%' || name.at(0) == '&') modules.insert({ name.substr(1, name.size() - 1), lineModule });
		else modules.insert({ name, lineModule });
	}

	// Initializes the recent pulses for the conjunction modules
	for (const auto& [name, module] : modules) {
		for (const auto& output : module.connected) {
			if (!modules.contains(output)) continue;
			if (modules.at(output).type != '&') continue;
			modules.at(output).recentPulses.insert({ name, false });
		}
	}

	// Pushes the button a thousand times
	int numLowPulses = 0, numHighPulses = 0;
	for (size_t i = 0; i < 1000; ++i) {
		auto [lowPulses, highPulses] = pressButton(modules);
		numLowPulses += lowPulses;
		numHighPulses += highPulses;
	}
	int product = numLowPulses * numHighPulses;

	std::cout << product << std::endl;

	return 0;
}
