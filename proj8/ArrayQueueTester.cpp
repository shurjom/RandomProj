/* QueueTester.cpp defines the test-methods for class Queue.
 * Joel Adams, for CS 112 at Calvin College.
 * Student name: Shurjo Maitra
 * Date: 4.5.16
 */
 
#include "ArrayQueueTester.h"

using namespace std;

void ArrayQueueTester::runTests() {
	cout << "Testing class ArrayQueue..." << endl;
	testConstructor();
	testAppend();
	testRemove();
	testCopyConstructor();
	testAssignment();
	cout << "All tests passed!\n" << endl;
}

void ArrayQueueTester::testAssignment() {
	cout << "- testing assignment... " << flush;
	// empty, smaller-to-bigger;
	ArrayQueue q1(4);
	ArrayQueue q2(3);
	q1 = q2;
	assert( q1.isEmpty() );
	assert( q1.myCapacity == 3 );
	assert( q1.getSize() == 0 );
	assert( q1.myFirst == q2.myFirst );
	assert( q1.myLast == q2.myLast );
	assert( q1.myArray != q2.myArray );
	cout << " 1 " << flush;
	//empty, bigger to smaller
	ArrayQueue q3(3);
	ArrayQueue q4(4);
	q3 = q4;
	assert( q3.isEmpty() );
	assert( q3.myCapacity == 4 );
	assert( q3.getSize() == 0 );
	assert( q3.myFirst == q4.myFirst );
	assert( q3.myLast == q4.myLast );
	assert( q3.myArray != q4.myArray );
	cout << " 2 " << flush;
	// full, same-sized
	ArrayQueue q5(4);
	q5.append(11);
	q5.append(22);
	q5.append(33);
	q5.append(44);
	ArrayQueue q6(4);
	q6 = q5;
	assert( q6.isFull() );
	assert( !q6.isEmpty() );
	assert( q6.myCapacity == 4 );
	assert( q6.getSize() == 4 );
	assert( q6.myFirst == q5.myFirst );
	assert( q6.myLast == q5.myLast );
	for (unsigned i = 0; i < q6.myCapacity; i++) {
		assert( q6.myArray[i] == q5.myArray[i] );
	}
	assert( q6.myArray != q5.myArray );
	cout << " 3 " << flush;
	// full, smaller-to-larger
	ArrayQueue q7(3);
	q7 = q5;
	assert( q7.isFull() );
	assert( !q7.isEmpty() );
	assert( q7.myCapacity == 4 );
	assert( q7.getSize() == 4 );
	assert( q7.myFirst == q5.myFirst );
	assert( q7.myLast == q5.myLast );
	for (unsigned i = 0; i < q7.myCapacity; i++) {
		assert( q7.myArray[i] == q5.myArray[i] );
	}
	assert( q7.myArray != q5.myArray );
	cout << " 4 " << flush;
	// full, larger-to-smaller
	ArrayQueue q8(5);
	q8 = q5;
	assert( q8.isFull() );
	assert( !q8.isEmpty() );
	assert( q8.myCapacity == 4 );
	assert( q8.getSize() == 4 );
	assert( q8.myFirst == q5.myFirst );
	assert( q8.myLast == q5.myLast );
	for (unsigned i = 0; i < q8.myCapacity; i++) {
		assert( q8.myArray[i] == q5.myArray[i] );
	}
	assert( q8.myArray != q5.myArray );
	cout << " 5 " << flush;
	// self assignment
	q5 = q5;
	assert( q5.isFull() );
	assert( !q5.isEmpty() );
	assert( q5.myCapacity == 4 );
	assert( q5.getSize() == 4 );
	assert( q5.myArray[0] == 11 );
	assert( q5.myArray[1] == 22 );
	assert( q5.myArray[2] == 33 );
	assert( q5.myArray[3] == 44 );
	cout << " 6 " << flush;
	// chaining
	q1 = q3 = q5;
	assert( q1.isFull() );
	assert( !q1.isEmpty() );
	assert( q1.myCapacity == 4 );
	assert( q1.getSize() == 4 );
	assert( q1.myArray[0] == 11 );
	assert( q1.myArray[1] == 22 );
	assert( q1.myArray[2] == 33 );
	assert( q1.myArray[3] == 44 );
	assert( q1.myArray != q3.myArray );
	assert( q1.myArray != q5.myArray );
	assert( q3.myArray != q5.myArray );
	cout << " 7 " << flush;
	cout << "Passed!" << endl;
}

void ArrayQueueTester::testCopyConstructor() {
	cout << "- testing copy constructor... " << flush;
	// empty queue
	ArrayQueue q1(4);
	ArrayQueue q2(q1);
	assert( q2.isEmpty() );
	assert( !q2.isFull() );
	assert( q2.myCapacity == q1.myCapacity);
	assert( q2.myFirst == q1.myFirst );
	assert( q2.myLast == q1.myLast );
	assert( q2.myArray != q1.myArray );
	cout << " 1 " << flush;
	// queue of 1 item
	ArrayQueue q3(4);
	q3.append(11);
	ArrayQueue q4(q3);
	assert( !q4.isFull() );
	assert( !q4.isEmpty() );
	assert( q4.myCapacity == q3.myCapacity);
	assert( q4.myFirst == q3.myFirst );
	assert( q4.myLast == q3.myLast );
	assert( q4.myArray != q3.myArray );
	assert( q4.getFirst() == 11 );
	assert( q4.getLast() == 11 );
	assert( q4.myArray != q3.myArray );
 	cout << " 2 " << flush;
 	// full queue
 	q3.append(22);
 	q3.append(33);
 	q3.append(44);
 	ArrayQueue q5(q3);
	assert( q5.isFull() );
	assert( !q5.isEmpty() );
	assert( q5.myCapacity == q3.myCapacity);
	assert( q5.myFirst == q3.myFirst );
	assert( q5.myLast == q3.myLast );
	assert( q5.myArray != q3.myArray );
 	assert( q5.getFirst() == 11 );
	assert( q5.getLast() == 44 );
	assert( q5.myArray != q3.myArray );
 	cout << " 3 " << flush;
	cout << "Passed!" << endl;
}

void ArrayQueueTester::testRemove() {
	cout << "- testing remove()... " << flush;
	ArrayQueue q1(4);
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
	// load it up
	q1.append(111);
	q1.append(222);
	q1.append(333);
	q1.append(444);
	assert( q1.isFull() );
	assert( !q1.isEmpty() );
	assert( q1.getFirst() == 111 );
	assert( q1.getLast() == 444 );
	// start removing items
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
	// recheck empty queue
	try {
		q1.remove();
		cerr << "\nremove() worked on empty Queue - 2\n";
		exit(1);
	} catch (EmptyQueueException& eqe) {
		cout << " 5 " << flush;
	}
	cout << "Passed!" << endl;
}

void ArrayQueueTester::testAppend() {
	cout << "- testing append(), getFirst(), getLast() ... " << flush;
	ArrayQueue q1(4);
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
	assert( q1.isFull() );
	assert( q1.getFirst() == 11 );
	assert( q1.getLast() == 44 );
	cout << " 4 " << flush;
	// append to full queue
	try {
		q1.append(55);
		cerr << "\nappend() worked on full queue\n";
		exit(1);
	} catch (FullQueueException& fqe) {
		cout << " 5 " << flush;
	}
	cout << "Passed!" << endl;
}

void ArrayQueueTester::testConstructor() {
	cout << "- testing constructor... " << flush;
	// try with capacity zero
	try {
		ArrayQueue q1(0);
		cerr << "Constructed Queue with capacity zero" << endl;
		exit(1);
	} catch (QueueException& qe) {
	    cout << " 1 " << flush;
	}

	// positive capacity
	ArrayQueue q2(3);
	assert( q2.isEmpty() );
	assert( !q2.isFull() );
	assert( q2.myCapacity == 3 );
	cout << " 2 " << flush;
	cout << " Passed!" << endl;
}

