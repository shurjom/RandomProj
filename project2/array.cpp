/* array.cpp defines "C style" array operations
 * Name:
 * Date:
 * Begun by: Joel Adams, for CS 112 at Calvin College.
 */
 
#include "array.h"

void initialize(double *a, int size) {
	int val = 0;
	for (int i = 0; i < size; i++) {
		val = i+1;
		a[i] = val;
	}
}
 
void print(double *a, int size) {
	for (int i = 0; i < size; i++) {
		cout << *a << '\t';
		a++;
	}
}		

double average(double *a, int size) {
	double sum = 0;
	for (int x = 0; x < size; x++) {
		sum += *a;
		a++;

	}
	return sum/size;
}

double sum(double *a, int size) {
	double sum = 0;
	for (int x = 0; x < size; x++) {
		sum += *a;
		a++;
	}
	return sum;
}
