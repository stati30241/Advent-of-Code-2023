#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>


int main() {
	// Opens the file
	std::ifstream ifs{ "input.txt" };
	if (!ifs.is_open()) return -1;

	// Reads in the time and the distance
	int64_t time = 0, distance = 0;
	std::string timeLine = "", distanceLine = "";
	std::getline(ifs, timeLine);
	std::getline(ifs, distanceLine);
	timeLine.erase(std::remove_if(timeLine.begin(), timeLine.end(), [](char ch) { return !isdigit(ch); }));
	distanceLine.erase(std::remove_if(distanceLine.begin(), distanceLine.end(), [](char ch) { return !isdigit(ch); }));
	time = std::atoll(timeLine.c_str());
	distance = std::atoll(distanceLine.c_str());

	// Finds the number of ways you can beat the record
	// The quadratic formula (^_^)
	// Have to use long doubles bc *precision*
	long double a = (-time + std::sqrt(time * time - 4.0l * distance)) / (-2.0l);
	long double b = (-time - std::sqrt(time * time - 4.0l * distance)) / (-2.0l);
	
	int64_t ways = 0;
	if (a == std::truncl(a)) ways = static_cast<int64_t>(std::abs(std::truncl(a) - std::truncl(b)) - 1);
	else ways = static_cast<int64_t>(std::abs(std::truncl(a) - std::truncl(b)) );

	std::cout << ways << std::endl;

	return 0;
}
