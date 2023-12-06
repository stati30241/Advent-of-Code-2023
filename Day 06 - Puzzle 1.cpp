#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


int main() {
	// Opens the file
	std::ifstream ifs{ "input.txt" };
	if (!ifs.is_open()) return -1;

	// Reads in the times
	std::vector<int> times, distances;
	std::string num = "";
	std::string timesLine = "";
	std::getline(ifs, timesLine);
	std::stringstream timesLineStream{ timesLine };
	while (timesLineStream >> num) {
		if (!isdigit(num.front())) continue;
		times.push_back(std::atoi(num.c_str()));
	}

	// Reads in the distances
	std::string distancesLine = "";
	std::getline(ifs, distancesLine);
	std::stringstream distancesLineStream{ distancesLine };
	while (distancesLineStream >> num) {
		if (!isdigit(num.front())) continue;
		distances.push_back(std::atoi(num.c_str()));
	}

	// Finds the product of the ways you can beat the record
	int prod = 1;
	for (size_t i = 0; i < times.size(); ++i) {
		// The quadratic formula >=)
		float a = (-times.at(i) + std::sqrtf(times.at(i) * times.at(i) - 4.0f * distances.at(i))) / (-2.0f);
		float b = (-times.at(i) - std::sqrtf(times.at(i) * times.at(i) - 4.0f * distances.at(i))) / (-2.0f);
		
		prod *= (a == std::truncf(a)) ? std::abs(std::truncf(a) - std::truncf(b)) - 1 : std::abs(std::truncf(a) - std::truncf(b));
	}

	std::cout << prod << std::endl;

	return 0;
}
