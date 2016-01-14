#ifndef LISTNODE_H
#define LISTNODE_H
template <typename NODETYPE> class List;

template <typename NODETYPE>
class ListNode
{
	friend class List<NODETYPE>;
public:
	ListNode(const NODETYPE &);
	NODETYPE getData() const;
	ListNode <NODETYPE> * getNextPtr() const;
	void setNextPtr(ListNode <NODETYPE> * val);
private:
	NODETYPE data;
	ListNode <NODETYPE> *nextPtr;
};

template <typename NODETYPE>
ListNode <NODETYPE>* ListNode<NODETYPE>::getNextPtr() const
{
	return nextPtr;
}

template <typename NODETYPE>
void ListNode<NODETYPE>::setNextPtr(ListNode <NODETYPE> * val)
{
	nextPtr = val;
}

template <typename NODETYPE>
ListNode<NODETYPE>::ListNode(const NODETYPE &info)
	: data(info), nextPtr(0)
{
}

template <typename NODETYPE>
NODETYPE ListNode< NODETYPE >::getData() const
{
	return data;
}

#endif

