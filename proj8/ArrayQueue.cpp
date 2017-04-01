/* ArrayQueue.cpp defines the methods for class ArrayQueue.
 * Joel Adams, for CS 112 at Calvin College.
 * Student name: Shurjo Maitra
 * Date: 4.5.16
 */

#include "ArrayQueue.h"
#include <cstring>      // memcpy()
using namespace std;

ArrayQueue::ArrayQueue(const ArrayQueue& original) {
	makeCopyOf(original);
}

void ArrayQueue::makeCopyOf(const ArrayQueue& original) {
	mySize = original.mySize;
	myCapacity = original.myCapacity;
	myFirst = original.myFirst;
	myLast = original.myLast;
	myArray = new Item[myCapacity];
	memcpy(myArray, original.myArray, myCapacity * sizeof(Item));
}

ArrayQueue::~ArrayQueue() {
	delete[] myArray;
	myArray = NULL;
	mySize = myFirst = myLast = 0;
}

ArrayQueue& ArrayQueue::operator=(const ArrayQueue& aQueue) {
	if (this != &aQueue) {
		delete[] myArray;
		makeCopyOf(aQueue);
	}
	return *this;
}

bool ArrayQueue::isEmpty() const {
	return mySize == 0;
}

bool ArrayQueue::isFull() const {
	return getSize() == myCapacity;
}

unsigned ArrayQueue::getCapacity() const {
	return myCapacity;
}

unsigned ArrayQueue::getSize() const {
	return mySize;
}

/*ArrayQueue constructor makes an Array of certain size
 *@param none
 *Postcondition: an array with size of input value
 */
ArrayQueue::ArrayQueue(unsigned capacity) {
	if (capacity > 0) {
		myCapacity = capacity;
		mySize = myFirst = myLast = 0;
		Item * newArray = new Item[capacity];
		myArray = newArray;
	} else {
		throw QueueException("Thrown at line 63:", "Queue capacity must be greater than 0.");
	}
}

/*myFirst accessor finds and returns the first value in the array
 *@param: none
 *Postcondition: the first value in the array is returned to the ostream
 */
Item ArrayQueue::getFirst() const {
	if (mySize == 0) {
		throw EmptyQueueException("Thrown at line 70");
	} else {
		return myArray[myFirst];
	}
}

/*myLast accessor finds and returns the last value in the array
 *@param:none
 *Postcondition: the last item in the array is returned to ostream
 */
Item ArrayQueue::getLast() const {
	if (mySize == 0) {
		throw EmptyQueueException("Thrown at line 81:");
	} else {
		return myArray[((myLast - 1) + myCapacity) % myCapacity];
	}
}

/* append method appends a new item to the end of the array
 * @param: Item from input
 * Postcondition: array with additional item at end and size refreshed
 */
void ArrayQueue::append(const Item& newItem) {
	if (mySize == myCapacity) {
		throw FullQueueException("Thrown at line 95:");
	} else {
		myArray[myLast] = newItem;
		myLast = (myLast + 1) % myCapacity;
		mySize++;
	}
}

/* remove() method removes and returns first item on array
 * @param: takes in item (from node)
 * Postcondition: item in the array that the node was
 *  pointing to is removed and returned
 */
Item& ArrayQueue::remove() {
	if (mySize == 0) {
		throw EmptyQueueException("Thrown at line 108:");
	} else {
		Item& result = myArray[myFirst];
		myFirst = (myFirst + 1) % myCapacity;
		mySize = mySize - 1;
		return result;
	}
}

