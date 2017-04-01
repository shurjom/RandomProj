#include "LinkedQueueTester.h"

#include <iostream>
#include <iostream>
#include <cassert>
#include<cstdlib>
using namespace std;

void LinkedQueueTester::runTests() {
	cout << "Testing class LinkedQueue..." << endl;
	testConstructor();
	testAppend();
	testRemove();
	testCopyConstructor();
	testAssignment();
	cout << "All tests passed!\n" << endl;
}

void LinkedQueueTester::testAssignment() {
	cout << "- testing assignment... " << flush;
	// empty-to-empty;
	LinkedQueue q1;
	LinkedQueue q2;
	q1 = q2;
	assert( q1.isEmpty() );
	assert( !q1.isFull() );
	assert( q1.getSize() == 0 );
	assert( q1.myFirst == NULL );
	assert( q1.myLast == NULL );
	cout << " 1 " << flush;
	//non-empty to empty
	LinkedQueue q3;
	LinkedQueue q4;
	q4.append(11);
	q4.append(22);
	q4.append(33);
	q3 = q4;
	assert( !q3.isEmpty() );
	assert( !q3.isFull() );
	assert( q3.getSize() == 3 );
	assert( q3.getFirst() == q4.getFirst() );
	assert( q3.getLast() == q4.getLast() );
	assert( q3.myFirst != q4.myFirst );
	assert( q3.myLast != q4.myLast );
	cout << " 2 " << flush;
	// empty to non-empty
	LinkedQueue q5;
	q5.append(11);
	q5.append(22);
	q5.append(33);
	q5.append(44);
	LinkedQueue q6;
	q5 = q6;
	assert( !q5.isFull() );
	assert( q5.isEmpty() );
	assert( q5.getSize() == 0 );
	assert( q5.myFirst == NULL );
	assert( q5.myLast == NULL );
	cout << " 3 " << flush;
	// both non-empty, larger-to-smaller
	LinkedQueue q7;
	q7.append(11);
	LinkedQueue q8;
	q8.append(22);
	q8.append(33);
	q8.append(44);
	q7 = q8;
	assert( !q7.isFull() );
	assert( !q7.isEmpty() );
	assert( q7.getSize() == 3 );
	assert( q7.myFirst != q8.myFirst );
	assert( q7.myLast != q8.myLast );
	assert( q7.getFirst() == q8.getFirst() );
	assert( q7.getLast() == q8.getLast() );
	cout << " 4 " << flush;
	// both non-empty, smaller-to-larger
	LinkedQueue q9;
	q9.append(55);
	q9.append(66);
	q7 = q9;
	assert( !q7.isFull() );
	assert( !q7.isEmpty() );
	assert( q7.getSize() == 2 );
	assert( q7.myFirst != q9.myFirst );
	assert( q7.myLast != q9.myLast );
	assert( q7.getFirst() == q9.getFirst() );
	assert( q7.getLast() == q9.getLast() );
	cout << " 5 " << flush;
	// self assignment
	q8 = q8;
	assert( !q8.isFull() );
	assert( !q8.isEmpty() );
	assert( q8.getSize() == 3 );
	assert( q8.getFirst() == 22 );
	assert( q8.myFirst->myNext->myItem == 33 );
	assert( q8.getLast() == 44 );
	cout << " 6 " << flush;
	// chaining
	q1 = q5 = q8;
	assert( !q1.isFull() );
	assert( !q1.isEmpty() );
	assert( q1.getSize() == 3 );
	assert( q1.getFirst() == 22 );
	assert( q1.myFirst->myNext->myItem == 33 );
	assert( q1.getLast() == 44 );
	assert( q1.myFirst != q5.myFirst );
	assert( q1.myFirst != q8.myFirst );
	assert( q5.myFirst != q8.myFirst );
	assert( q1.myLast != q5.myLast );
	assert( q1.myLast != q8.myLast );
	assert( q5.myLast != q8.myLast );
	cout << " 7 " << flush;
	cout << "Passed!" << endl;
}

void LinkedQueueTester::testCopyConstructor() {
	cout << "- testing copy constructor... " << flush;
	// empty queue
	LinkedQueue q1;
	LinkedQueue q2(q1);
	assert( q2.isEmpty() );
	assert( !q2.isFull() );
	assert( q2.myFirst == NULL );
	assert( q2.myLast == NULL );
	assert( q2.getSize() == 0 );
	cout << " 1 " << flush;
	// queue of 3 items
	LinkedQueue q3;
	q3.append(11);
	q3.append(22);
	q3.append(33);
	LinkedQueue q4(q3);
	assert( !q4.isFull() );
	assert( !q4.isEmpty() );
	assert( q4.myFirst != q3.myFirst );
	assert( q4.myLast != q3.myLast );
	assert( q4.getFirst() == 11 );
	assert( q4.myFirst->myNext->myItem == 22 );
	assert( q4.getLast() == 33 );
	assert( q4.getSize() == 3 );
	assert( q4.myFirst != q3.myFirst );
	assert( q4.myLast != q3.myLast );
 	cout << " 2 " << flush;
	cout << "Passed!" << endl;
}

void LinkedQueueTester::testRemove() {
	cout << "- testing remove()... " << flush;
	LinkedQueue q1;
	cout << " 0 " << flush;
	// check with empty queue
	try {
		q1.remove();
		cerr << "\nremove() worked on empty Queue - 1\n";
		exit(1);
	} catch (EmptyQueueException& eqe) {
		cout << " 1 " << flush;
	}
	// check with 1 item
	q1.append(1);
	assert( !q1.isEmpty() );
	assert( q1.remove() == 1 );
	assert( q1.isEmpty() );
	cout << " 2 " << flush;
	// check with 2 items
	q1.append(11);
	q1.append(22);
	assert( !q1.isEmpty() );
	assert( q1.getFirst() == 11 );
	assert( q1.getLast() == 22 );
	assert( q1.remove() == 11 );
	assert( q1.getFirst() == 22 );
	assert( q1.getLast() == 22 );
	assert( q1.remove() == 22 );
	assert( q1.isEmpty() );
	cout << " 3 " << flush;
	// try several values
	q1.append(111);
	q1.append(222);
	q1.append(333);
	q1.append(444);
	assert( !q1.isFull() );
	assert( !q1.isEmpty() );
	assert( q1.getFirst() == 111 );
	assert( q1.getLast() == 444 );
	// now start removing them
	assert( q1.remove() == 111 );
	assert( !q1.isFull() );
	assert( !q1.isEmpty() );
	assert( q1.getFirst() == 222 );
	assert( q1.getLast() == 444 );
    cout << " 4a " << flush;
	assert( q1.remove() == 222 );
	assert( !q1.isFull() );
	assert( !q1.isEmpty() );
	assert( q1.getFirst() == 333 );
	assert( q1.getLast() == 444 );
    cout << " 4b " << flush;
	assert( q1.remove() == 333 );
	assert( !q1.isFull() );
	assert( !q1.isEmpty() );
	assert( q1.getFirst() == 444 );
	assert( q1.getLast() == 444 );
    cout << " 4c " << flush;
	assert( q1.remove() == 444 );
	assert( !q1.isFull() );
	assert( q1.isEmpty() );
    cout << " 4d " << flush;
	// recheck with empty queue
	try {
		q1.remove();
		cerr << "\nremove() worked on empty Queue - 2\n";
		exit(1);
	} catch (EmptyQueueException& eqe) {
		cout << " 5 " << flush;
	}
	cout << "Passed!" << endl;
}

void LinkedQueueTester::testAppend() {
	cout << "- testing append(), getFirst(), getLast(), ... " << flush;
	LinkedQueue q1;
	assert( q1.isEmpty() );
	assert( !q1.isFull() );
	try {
		q1.getFirst();
		cerr << "\ngetFirst() worked on empty queue\n";
		exit(1);
	} catch (EmptyQueueException& eqe) {
		cout << " 0a " << flush;
	}
	try {
		q1.getLast();
		cerr << "\ngetLast() worked on empty queue\n";
		exit(1);
	} catch (EmptyQueueException& eqe) {
		cout << " 0b " << flush;
	}
	q1.append(11);
	assert( !q1.isEmpty() );
	assert( !q1.isFull() );
	assert( q1.getFirst() == 11 );
	assert( q1.getLast() == 11 );
	cout << " 1 " << flush;
	q1.append(22);
	assert( !q1.isEmpty() );
	assert( !q1.isFull() );
	assert( q1.getFirst() == 11 );
	assert( q1.getLast() == 22 );
	cout << " 2 " << flush;
	q1.append(33);
	assert( !q1.isEmpty() );
	assert( !q1.isFull() );
	assert( q1.getFirst() == 11 );
	assert( q1.getLast() == 33 );
	cout << " 3 " << flush;
	q1.append(44);
	assert( !q1.isEmpty() );
	assert( !q1.isFull() );
	assert( q1.getFirst() == 11 );
	assert( q1.getLast() == 44 );
	cout << " 4 " << flush;
	cout << "Passed!" << endl;
}

void LinkedQueueTester::testConstructor() {
	cout << "- testing constructor... " << flush;
	// linked queues have no capacity
	LinkedQueue q1;
	assert( q1.getSize() == 0 );
	assert( q1.myFirst == NULL );
	assert( q1.myLast == NULL );
 	cout << " Passed!" << endl;
}

