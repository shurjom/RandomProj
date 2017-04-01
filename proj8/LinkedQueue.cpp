/* LinkedQueue.cpp defines the methods for class LinkedQueue.
 * Joel Adams, for CS 112 at Calvin College.
 * Student name: Shurjo Maitra
 * Date: 4.5.16
 */

#include "LinkedQueue.h"


LinkedQueue::LinkedQueue(const LinkedQueue& original) {
	makeCopyOf(original);
}

void LinkedQueue::makeCopyOf(const LinkedQueue& original) {
	mySize = original.mySize;
	if ( mySize == 0 ) {
		myFirst = myLast = NULL;
	} else {
		myFirst = new Node(original.getFirst(), NULL);
		Node * temp0 = original.myFirst->myNext;
		Node * temp1 = myFirst;
		while (temp0 != NULL) {
			temp1->myNext = new Node(temp0->myItem, NULL);
			temp1 = temp1->myNext;
			temp0 = temp0->myNext;
		}
		myLast = temp1;
	}
}

LinkedQueue::~LinkedQueue() {
	delete myFirst;
	myFirst = myLast = NULL;
	mySize = 0;
}

LinkedQueue& LinkedQueue::operator=(const LinkedQueue& aQueue) {
	if (this != &aQueue) {
		delete myFirst;    // invokes recursive ~Node()
		makeCopyOf(aQueue);
	}
	return *this;
}



bool LinkedQueue::isFull() const {
	return false;
}

/* isEmpty method checks if the list is empty
 * @param: none
 * Postcondition: returns bool of whether list is empty
 */
bool LinkedQueue::isEmpty() const {
	return mySize == 0;
}


/* mySize accessor returns the no. of items in the list
 * @param: none
 * Postcondition: mySize is returned from the list
 */
unsigned LinkedQueue::getSize() const {
	return mySize;
}

/* LinkedQueue constructor makes and sets an empty list
 * @param: none
 * Postcondition: an empty list is returned
 */
LinkedQueue::LinkedQueue() {
	mySize = 0;
	myFirst = myLast = NULL;
}

/* myFirst accessor returns the first item in the list
 * @param: none
 * Postcondition: the first item in the list is returned to ostream
 */
Item LinkedQueue::getFirst() const {
	if (mySize == 0) {
		throw EmptyQueueException("Thrown at line 80: ");
	}
	else {
		return myFirst->myItem;
	}
}

/* myLast accessor returns the last item in the list
 * @param: none
 * Postcondition: the last item in the list is returned to ostream
 */
Item LinkedQueue::getLast() const {
	if (mySize == 0) {
		throw EmptyQueueException("Thrown at line 95: ");
	}
	else {
		return myLast->myItem;
	}
}

/* append method appends a new item to the end of the list
 * @param: Item from input
 * Postcondition: list with additional item at end and size refreshed
 */
void LinkedQueue::append(const Item& newitem){
	Node * ptr = new Node(newitem, NULL);
	if (mySize == 0){
		myFirst = ptr;
	}
	else {
		myLast->myNext = ptr;
	}
	myLast = ptr;
	mySize ++;
}

/* remove() method removes and returns first item on list
 * @param: takes in item (from node)
 * Postcondition: item in the list that the node was
 *  pointing to is removed and returned
 */
Item& LinkedQueue::remove() {
	if (mySize == 0) {
		throw EmptyQueueException("Thrown at line 123: ");
	}
	else {
	Item result = myFirst->myItem;
	Node * nptr = myFirst;
	myFirst = nptr->myNext;
	nptr->myNext = NULL;
	delete nptr;
	mySize = mySize - 1;
	return result;
	}
}

