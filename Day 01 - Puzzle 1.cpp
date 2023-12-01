#include <iostream>
#include <fstream>
#include <vector>


int main() {
	// Opens the input
	std::ifstream ifs{ "input.txt" };
	if (!ifs.is_open()) return -1;

	int sum = 0;
	std::vector<int> numbers;
	char ch;
	while (ifs.get(ch)) {
		// Adds the parsed number to the sum
		if (ch == '\n') {
			sum += numbers.front() * 10 + numbers.back();
			numbers.clear();
		}

		if (ch < 48 || ch > 57) continue;
		numbers.push_back(ch - 48);
	}
	sum += numbers.front() * 10 + numbers.back();

	std::cout << sum << std::endl;

	return 0;
}
