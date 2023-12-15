#include <fstream>
#include <iostream>
#include <vector>


// Simple struct to store the information about lenses
struct Lens {
	std::string name;
	int focalLenght;
};

bool operator== (const Lens& left, const Lens& right) {
	return left.name == right.name;
}


int main() {
	// Opens the file
	std::ifstream ifs{ "input.txt" };
	if (!ifs.is_open()) return -1;

	// Reads in the strings and stores the lenses in the given
	// boxes based on the instructions
	std::vector<std::vector<Lens>> boxes(256);
	char ch;
	std::string token = "";
	int hash = 0;
	while (ifs.get(ch)) {
		if (ch == '=') {
			// Reads in the focal length
			char focalLength;
			ifs.get(focalLength);

			// If the box with that label already exists, replace the focal length
			Lens lens{ token, focalLength - 48 };
			auto search = std::find(boxes.at(hash).begin(), boxes.at(hash).end(), lens);
			if (search != boxes.at(hash).end()) {
				*search = lens;
			} else {
				boxes.at(hash).push_back(lens);
			}

			// Ignores the comma and resets the token and hash
			ifs.seekg(1, std::ios_base::cur);
			hash = 0;
			token = "";
			continue;
		} else if (ch == '-') {
			// If the lens exists, remove it
			Lens lens{ token, 0 };
			auto search = std::find(boxes.at(hash).begin(), boxes.at(hash).end(), lens);
			if (search != boxes.at(hash).end()) boxes.at(hash).erase(search);

			// Ignores the comma and resets the token and hash
			ifs.seekg(1, std::ios_base::cur);
			hash = 0;
			token = "";
			continue;
		}

		// Updates the token and the hash
		token += ch;
		hash += static_cast<int>(ch);
		hash *= 17;
		hash %= 256;
	}

	// Calculates the sum of the focusing powers of each of the lenses
	int sum = 0;
	for (size_t i = 0; i < boxes.size(); ++i) {
		for (size_t j = 0; j < boxes.at(i).size(); ++j) {
			sum += (i + 1) * (j + 1) * boxes.at(i).at(j).focalLenght;
		}
	}

	std::cout << sum << std::endl;

	return 0;
}
