#pragma once
#include <string>
#include "ListNode.h"
using namespace std;
class QueueStr
{
public:
	QueueStr();
	~QueueStr();
	void enqueue(const string &);
	bool dequeue(string &);
	bool isQueueEmpty() const;
	void print() const;
private:
	ListNode <string> *firstPtr;
	ListNode <string> *lastPtr;

	ListNode <string> *getNewNode(const string &);
};
	

