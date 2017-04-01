/*
 * Age Responce.cpp
 *
 *  Created on: Jun 5, 2015
 *      Author: Shurjo
 */

/*
 * Funny age respnse.cpp
 *
 *  Created on: Jun 5, 2015
 *      Author: Shurjo
 */

#include <iostream>
#include <math.h>
using namespace std;

int main()
{
	int age = round(45.5);
    cout << "Your are this age but change it!";
    cout << "\nYou are:" << age;
    cout << "\nWhat is your age really? ";
	cin >> age;
	cout << "Your age is: " << age;
	if (age < 21 )
		cout << "\nYou can't get a drink";
	else if (age == 21)
		cout << "\nNow you can finally drink!";
	else if (age <= 50)
		cout << "\nYou can get a drink";
	else if (age < 150)
		cout << "\nStop Drinking!";
	else if (age < 2000)
		cout << "\nYou should be dead!";
	else
		cout << "\nGo drink blood you're a Vampire!";
    return 0;
}








