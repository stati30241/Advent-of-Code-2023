#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <unordered_map>


// Simple struct to keep track of the cards
struct Hand {
	static const size_t numCards = 5;
	std::string cards;
	int bid;
};


// Compares two cards
// Behold this ugly, diseased solution that my cursed mind managed to
// cook up for today's challege. Yes is works, but is very unelegant
bool operator< (const Hand& left, const Hand& right) {
	static const std::array<char, 13> cardValues{'J', '2', '3', '4', '5', '6',
		'7', '8', '9', 'T', 'Q', 'K'};

	// Calculates the value of the hands
	std::unordered_map<char, int> leftDistinctCards, rightDistinctCards;
	for (size_t i = 0; i < Hand::numCards; ++i) {
		leftDistinctCards.insert({ left.cards.at(i), std::count(left.cards.begin(), left.cards.end(), left.cards.at(i)) });
		rightDistinctCards.insert({ right.cards.at(i), std::count(right.cards.begin(), right.cards.end(), right.cards.at(i)) });
	}
	
	// Makes use of the wildcard property of J by just making it be
	// the card that has the most amount of it
	char maxLeftKey = '\0', maxRightKey = '\0';
	for (const auto key : leftDistinctCards) {
		if (key.first == 'J') continue;
		if (maxLeftKey == '\0') maxLeftKey = key.first;
		if (key.second > leftDistinctCards.at(maxLeftKey)) maxLeftKey = key.first;
	}
	for (const auto key : rightDistinctCards) {
		if (key.first == 'J') continue;
		if (maxRightKey == '\0') maxRightKey = key.first;
		if (key.second > rightDistinctCards.at(maxRightKey)) maxRightKey = key.first;
	}
	if (maxLeftKey != '\0' && leftDistinctCards.contains('J')) {
		leftDistinctCards.at(maxLeftKey) += leftDistinctCards.at('J');
		leftDistinctCards.erase('J');
	}
	if (maxRightKey != '\0' && rightDistinctCards.contains('J')) {
		rightDistinctCards.at(maxRightKey) += rightDistinctCards.at('J');
		rightDistinctCards.erase('J');
	}

	if (leftDistinctCards.size() != rightDistinctCards.size()) return leftDistinctCards.size() > rightDistinctCards.size();

	size_t leftMaxDifference = 0, rightMaxDifference = 0;
	for (const auto i : leftDistinctCards) {
		for (const auto j : leftDistinctCards) {
			if (std::abs(i.second - j.second) > leftMaxDifference) leftMaxDifference = std::abs(i.second - j.second);
		}
	}
	for (const auto i : rightDistinctCards) {
		for (const auto j : rightDistinctCards) {
			if (std::abs(i.second - j.second) > rightMaxDifference) rightMaxDifference = std::abs(i.second - j.second);
		}
	}
	if (leftMaxDifference != rightMaxDifference) return leftMaxDifference < rightMaxDifference;

	// Calculates the values of each of the cards
	for (size_t i = 0; i < Hand::numCards; ++i) {
		size_t leftCardIndex = std::distance(cardValues.begin(), std::find(cardValues.begin(), cardValues.end(), left.cards.at(i)));
		size_t rightCardIndex = std::distance(cardValues.begin(), std::find(cardValues.begin(), cardValues.end(), right.cards.at(i)));
		if (leftCardIndex != rightCardIndex) return leftCardIndex < rightCardIndex;
	}

	return false;
}


int main() {
	// Opens the file
	std::ifstream ifs{ "input.txt" };
	if (!ifs.is_open()) return -1;

	// Reads in the hands and the bids
	std::vector<Hand> hands;
	std::string cards = "";
	int bid = 0;
	while (ifs >> cards >> bid) {
		hands.push_back({ cards, bid });
	}

	// Calculates the total winning
	int winnings = 0;
	std::sort(hands.begin(), hands.end());
	for (size_t i = 0; i < hands.size(); ++i) {
		winnings += (i + 1) * hands.at(i).bid;
	}

	std::cout << winnings << std::endl;

	return 0;
}
