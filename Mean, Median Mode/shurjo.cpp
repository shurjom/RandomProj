/*
 * shurjo.cpp
 *
 *  Created on: Dec 19, 2014
 *      Author: Shurjo
 */

#include <iostream>
#include <math.h>
#include <vector>
#include <sstream>
#include <string>

using namespace std;

typedef vector<double> numbers_t;

/**s
 * Finds the mean from a vector
 *
 */
double mean(const numbers_t& numbers)
{
	double accum = 0;
	for (numbers_t::const_iterator i = numbers.begin(); i != numbers.end(); i++)
			accum += *i;
	return accum/numbers.size();
}


double mode(const numbers_t& numbers)
{
	if (numbers.size() % 2 == 1) // if numbers has a middle - only if the amount in numbers is odd then returnt he middle number
	{
		return numbers[numbers.size()/2];
	}

	// If there is an even amount of numbers than returns the mean of the two in the middle
	return numbers[numbers.size()/2] + numbers[numbers.size()/2 + 1] / 2;

}

// http://stackoverflow.com/a/19095024/2948122
template<typename T>
string str(T begin, T end)
{
	stringstream ss;
	for (; begin != end; begin++)
	{
		if (ss.str().size())
			ss << ", ";
		ss << *begin;
	}
	return ss.str();
}

/* This is a multiple line comment
 * multiple line comments are really nice to use because they don't force you to write the double forward slashes.
 */

int main()
{

	numbers_t numbers;
	while (true)
	{
		stringbuf userInput;
		cout << "ENTER NUMBER:";
		cin.get(userInput, '\n');
		cin.get(); // read past that '\n' that we didn't read before

		// Checks if user doesn't enter anything, if the user does then stop taking input
		if (userInput.str().size() == 0)
			break;

		numbers_t::value_type num;
		if ( !(stringstream(userInput.str()) >> num) )//if the input is not able to convert to a double then tell user to input a number and continue
		{
			cout << "Must Enter a number" << endl;
			continue;
		}
		numbers.push_back(num); //add the new number the user inputed to the vector<double> numbers
		cout << "Number List\"";
		cout << str(numbers.begin(), numbers.end()) << "\"" << endl;
	}
	std::sort(numbers.begin(), numbers.end(), numbers);
	cout << "NUMBER LIST:" << str(numbers.begin(), numbers.end());
	cout << "Mean = " << mean(numbers) << endl; // Print out the mean
	return 0;
}

/*
double asdf = 1234
vector<double> asdf();
asdf.push_back(54);


132,123,123,32,23,123,223
asdf.puch_back(900001)
132,123,123,32,23,123,223,900001
*/


