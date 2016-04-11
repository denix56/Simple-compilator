#ifndef STACK_H
#define STACK_H
#include "List.h"

template <typename STACKTYPE>
class Stack : private List<STACKTYPE>
{
public:
	void push(const STACKTYPE &data)
	{
		insertAtFront(data);
	}

	bool pop(STACKTYPE &data)
	{
		return removeFromFront(data);
	}

	bool isStackEmpty() const
	{
		return isEmpty();
	}

	void printStack() const
	{
		print();
	}

};
#endif