/* LinkedQueue.h declares a Queue class using linked nodes.
 * Begun by: Joel Adams, for CS 112 at Calvin College.
 * Student name: Shurjo Maitra
 * Date: 4.5.16
 *
 * 
 * Class Invariant: 
 *    mySize == 0 && myFirst == NULL && myLast == NULL ||
 *    mySize > 0 && myFirst == the address of a node containing the oldest item
 *               && myLast == the address of a node containing the newest item.
 */

#ifndef LINKED_QUEUE_H_
#define LINKED_QUEUE_H_

#include "QueueException.h"

typedef int Item;

class LinkedQueue {
public:

	LinkedQueue(const LinkedQueue& original);
	virtual ~LinkedQueue();
	LinkedQueue& operator=(const LinkedQueue& original);
	unsigned getSize() const;
	bool isEmpty() const;
	bool isFull() const;
	LinkedQueue();
	Item getFirst() const;
	Item getLast() const;
	void append(const Item& newitem);
	Item& remove();

protected:
	void makeCopyOf(const LinkedQueue& original);
private:
	unsigned mySize;
	struct Node {
		Item myItem;
		Node * myNext;
		
		Node(const Item& it, Node* next) { myItem = it; myNext = next; }
		~Node() { delete myNext; }
	};
	Node * myFirst;
	Node * myLast;
	friend class LinkedQueueTester;
};

#endif /*LINKED_QUEUE_H_*/
