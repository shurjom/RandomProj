/* ArrayQueueTester.h declares the class that tests class ArrayQueue.
 * Joel Adams, for CS 112 at Calvin College.
 * Student name: Shurjo Maitra
 * Date: 4.5.16
 */
 
#ifndef QUEUETESTER_H_
#define QUEUETESTER_H_

#include "ArrayQueue.h"
#include <cstdlib>
#include <iostream>
#include <cassert>

class ArrayQueueTester {
public:
	void runTests();
	void testConstructor();
	void testAppend();
	void testRemove();
	void testCopyConstructor();
	void testAssignment();
};

#endif /*QUEUETESTER_H_*/
