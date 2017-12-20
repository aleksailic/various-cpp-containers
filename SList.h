#pragma once
#include "List.h"
template <class T>
class SList :public List<T>
{
protected:
	using List::Node;
public:
	using List::List;
	SList() = default;
	SList(std::initializer_list<T> lst) {
		for (auto el : lst)
			add(el);
	}
	SList(List& lst){
		for (auto itr = lst.begin(); itr != lst.end(); ++itr)
			add(*itr);
	}
	void add(const T& el) {
		Node*nd = new Node(el);
		Node*before = nullptr;
		Node*after = front;
		for (; after != nullptr && (after->data) <= el; after = after->nxt)
			before = after;

		if (!after || after->data != el) //samo jedinstveno!
			insertNode(before, nd);
	}
	void print(std::ostream& os) {
		os << "SList(" << length << "):{";
		for (auto itr = begin(); itr != end(); ++itr) {
			os << *itr << ", ";
		}
		os << '}' << std::endl;
	}
};

