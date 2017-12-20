#pragma once
#include "Container.h"

template<class T>
class List :public Container<T> {
protected:
	struct Node {
		T data;
		Node*nxt;

		Node() = delete;
		Node(const T&el) :data(el), nxt(nullptr) {}
	};
	Node * front;
	Node * rear;

	mutable struct { //Kako bi operacije brisanja i pronalaska prethodnog bile optimalne ukoliko se pozivaju na prethodno korisceni element
		Node * el = nullptr;
		Node * parent = nullptr;
	}lastAccessed;

	class L_iter : public Iter<T> {
		Node* curr;

		void kopiraj(const Iter<T>& rhs) {
			const L_iter& lst = dynamic_cast<const L_iter&>(rhs);
			curr = lst.curr;
		}
	public:
		L_iter(Node*el) :curr(el) {}
		L_iter(const L_iter& itr) { kopiraj(itr); }

		Iter<T>& next() {
			curr = curr->nxt;
			return *this;
		}

		bool operator==(const Iter<T>& rhs) { const L_iter& lst = dynamic_cast<const L_iter&>(rhs); return curr == lst.curr ? 1 : 0; }
		T* operator->() { return &(curr->data); }
		Iter<T>& operator=(const Iter<T>& rhs) {
			if (this != &rhs) {
				brisi();
				kopiraj(rhs);
			}
			return *this;
		}
	};
	
	virtual void insertNode(Node* prev, Node* curr) {
		length++;
		if (!prev) { // umece se na mestu prvog
			curr->nxt = front;
			front = curr;
		}
		else {
			curr->nxt = prev->nxt;
			prev->nxt = curr;
		}
		if (!rear || !curr->nxt)
			rear = curr;
	}
	virtual Node* getPrevNode(Node *nd) const {
		if (!front)return nullptr;
		if (nd == front) return nullptr;
		if (nd == lastAccessed.el) return lastAccessed.parent;

		Node*itr = front;
		for (; itr->nxt != nd && itr->nxt != nullptr; itr = itr->nxt);
		return itr;
	}
	virtual void removeNode(Node * nd) {
		if (!nd) return;
		length--;
		Node * prev = getPrevNode(nd);

		if (prev)
			prev->nxt = nd->nxt;
		else
			front = nd->nxt;

		if (rear == nd) rear = prev;
		delete nd;
	}
	virtual Node* findNode(const T& el) const {
		if (lastAccessed.el && lastAccessed.el->data == el)
			return lastAccessed.el;

		lastAccessed.parent = nullptr;
		for (Node*itr = front; itr != nullptr; itr = itr->nxt)
			if (itr->data == el)
				return lastAccessed.el = itr;
			else
				lastAccessed.parent = itr;

		return nullptr;
	}
	Node* getNodeById(unsigned int id)const {
		Node*itr = front;
		lastAccessed.parent = nullptr;
		for (unsigned int i = 0; i < id; i++) {
			lastAccessed.parent = itr;
			itr = itr ? itr->nxt : nullptr;
		}
			
		return lastAccessed.el=itr;
	}

	virtual void kopiraj(const Container&c) {
		const List& l = dynamic_cast<const List&>(c);
		for (Node* itr = l.front; itr != nullptr; itr = itr->nxt)
			add(itr->data);
	}
	virtual void brisi() {
		rear = nullptr;
		length = 0;
		while (front) {
			Node * toDelete = front;
			front = front->nxt;
			delete toDelete;
		}
	}
	virtual void premesti(Container&&c) {
		List& l = dynamic_cast<List&>(c);
		front = l.front;
		rear = l.rear;
		length = l.length;

		l.front = nullptr; //samo front je dovoljan
	}
	virtual void print(std::ostream& os) {
		/*os << "List" <<"(" << length << "):{";
		for (auto itr = begin(); itr != end(); ++itr) {
			os << *itr << ", ";
		}
		os << '}' << std::endl;*/
	}
public:
	List() = default;
	List(std::initializer_list<T> lst) { //Lakse se je inicijalizovati inicijalizatorskom listom
		for (auto el : lst)
			add(el);
	}
	List(const List&l) { kopiraj(l); }
	List(List && l) { premesti(l); }

	List& operator=(const List&l) {
		if (this != &l) {
			brisi();
			kopiraj(l);
		}
		return *this;
	}

	virtual ~List() { brisi(); }//preuzmi destruktor

	virtual void add(const T& el) {
		Node*nd = new Node(el);
		insertNode(rear, nd);
	}
	T pop() {
		T data = rear->data;
		removeNode(rear);
		return data;
	}
	void remove(const T&data) { removeNode(findNode(data)); }
	bool has(const T& data)const { return findNode(data) ? 1 : 0; }
	T fetch(unsigned int id){
		if (id >= length) return T();
		Node * el = getNodeById(id);
		return el ? el->data : T();
	}

	void forEach(void(*fn)(T& data)) {
		for (Node*itr = front; itr != nullptr; itr = itr->nxt)
			fn(itr->data);
	}

	L_iter begin() {
		return L_iter(front);
	}
	L_iter end() {
		return L_iter(nullptr);
	}
};