#include <iostream>
#include <string>
#include <fstream>
#include <vector>

template <class Item>
void outputVector(const std::vector<Item> &vectorOfInts, std::ostream& output) {
	for (auto i = vectorOfInts.cbegin(); i != vectorOfInts.cend(); i++) {
		output << *i << std::endl;
		output << "huh" << std::endl;
	}
	output << "It has been printed" << std::endl;

}

int main() {
	std::cout << "helloe" << std::endl;
	for (int i = 0; i < 10; i++) {
		std::cout << "this is repeated exactly 10 times" << std::endl;
	}
	std::string funness = "This is dumb";
	funness = funness + "this is awesome";
	std::cout << funness;
	std::vector<int> vectorOfInts;
	vectorOfInts.push_back(10);
	vectorOfInts.push_back(123);
	vectorOfInts.push_back(12321321);
	vectorOfInts.push_back(21443);
	outputVector(vectorOfInts, std::cout);

	std::cout << "This is the end" << std::endl;
}
