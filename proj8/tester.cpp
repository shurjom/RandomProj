/* tester.cpp drives the testing of the Queue classes.
 * Joel Adams, for CS 112 at Calvin College.
 * Student name: Shurjo Maitra
 * Date: 4.5.16
 */

 #include "ArrayQueueTester.h"
 #include "LinkedQueueTester.h"
 
 int main() {
 	ArrayQueueTester aqt;
 	aqt.runTests();
 	LinkedQueueTester lqt;
 	lqt.runTests();
 }
 
 
