#include <iostream>
using std::cout;
#include "QueueStr.h"



QueueStr::QueueStr() : firstPtr(0), lastPtr(0)
{
}


QueueStr::~QueueStr()
{
	if (!isQueueEmpty())
	{
		//cout << "Destroying nodes...\n";

		ListNode <string> *currentPtr = firstPtr;
		ListNode <string> *tempPtr;
		while (currentPtr != 0)
		{
			tempPtr = currentPtr;
			//cout << tempPtr->data << '\n';
			currentPtr = currentPtr->getNextPtr();
			delete tempPtr;
		}
	}
}




void QueueStr::enqueue(const string &value)
{
	ListNode <string> *newPtr = getNewNode(value);

	if (isQueueEmpty())
		firstPtr = lastPtr = newPtr;
	else
	{
		lastPtr->setNextPtr(newPtr);
		lastPtr = newPtr;
	}
}

bool QueueStr::dequeue(string &value)
{
	if (isQueueEmpty())
		return false;
	else
	{
		ListNode <string> *tempPtr = firstPtr;

		if (firstPtr == lastPtr)
			firstPtr = lastPtr = 0;
		else
			firstPtr = firstPtr->getNextPtr();
		//if(typeid(NODETYPE) == typeid(string))
		//else
		value = tempPtr->getData().substr();
		delete tempPtr;
		return true;
	}
}


bool QueueStr::isQueueEmpty() const
{
	return firstPtr == 0;
}

ListNode <string> *QueueStr::getNewNode(const string &value)
{
	return new ListNode <string>(value);
}

void QueueStr::print() const
{
	if (isQueueEmpty())
	{
		cout << "The list is empty\n\n";
		return;
	}

	ListNode <string> *currentPtr = firstPtr;

	cout << "The list is: ";

	while (currentPtr != 0)
	{
		cout << currentPtr->getData() << ' ';
		currentPtr = currentPtr->getNextPtr();
	}
	cout << "\n\n";
}

