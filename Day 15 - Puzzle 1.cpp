#include <fstream>
#include <iostream>


int main() {
	// Opens the file
	std::ifstream ifs{ "input.txt" };
	if (!ifs.is_open()) return -1;

	int sum = 0, hash = 0;
	char ch;
	while (ifs.get(ch)) {
		// Ignore newlines
		if (ch == '\n') continue;

		// If its a comma, then its the end of the string
		if (ch == ',') {
			sum += hash;
			hash = 0;
			continue;
		}

		// Add to the hash of the string
		hash += static_cast<int>(ch);
		hash *= 17;
		hash %= 256;
	}
	sum += hash;

	std::cout << sum << std::endl;

	return 0;
}
