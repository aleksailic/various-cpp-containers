#pragma once
#include "Container.h"
#include "Stack.h"



template <class T>
class BST : public Container<T>
{
protected:
	struct Node {
		Node*left;
		Node*right;
		T data;

		Node() = delete;
		Node(const T& cdata) :data(cdata), left(nullptr), right(nullptr) {}
	};
	Node*root;


	mutable struct{ //Kako bi operacije brisanja i pronalaska prethodnog bile optimalne ukoliko se pozivaju na prethodno korisceni element
		Node * el=nullptr;
		Node * parent=nullptr;
	}lastAccessed;

	class BST_iter : public Iter<T> {
		Stack<Node*> s;
		Node* curr;
		Node* nxt;

		void kopiraj(const Iter<T>& rhs) {
			const BST_iter& bst = dynamic_cast<const BST_iter&>(rhs);
			curr = bst.curr;
			nxt = bst.nxt;
			s = bst.s;
		}
		void brisi() {s.erase();}
	public:
		BST_iter(Node*start) :nxt(start),curr(nullptr), s() { if (nxt)next(); }
		BST_iter(const BST_iter& itr) { kopiraj(itr); }


		Iter<T>& next() {
			while (nxt) { //idi skroz levo dok mozes
				s.push(nxt);
				nxt = nxt->left;
			}
			if (!s.empty()) {
				nxt = s.pop();
				curr = nxt;
				nxt = nxt->right;
			}
			else
				curr = nullptr; //dosli smo do kraja

			return *this;
		}

		bool operator==(const Iter<T>& rhs) {
			const BST_iter& bst = dynamic_cast<const BST_iter&>(rhs);
			return curr == bst.curr ? 1 : 0;
		}
		T* operator->() { return &(curr->data); }
		T& operator*() { return *operator->(); }
		Iter<T>& operator=(const Iter<T>& rhs) {
			if (this != &rhs) {
				brisi();
				kopiraj(rhs);
			}
			return *this;
		}
		void operator~() {
			delete curr;
			curr = nullptr;
		}
	};
private:
	virtual void kopiraj(const Container&c) {
		const BST& cb = dynamic_cast<const BST&>(c);
		BST& b = const_cast<BST&>(cb);
		for (auto itr = b.begin(); itr != b.end(); ++itr)
			add(*itr);
	}
	virtual void brisi() {
		for (auto itr = begin(); itr != end(); ++itr)
			~itr;
		root = nullptr;
		length = 0;
	}
	virtual void premesti(Container&&c) {
		BST& b = dynamic_cast<BST&>(c);
		root = b.root;
		b.root = nullptr;
	}
	virtual void print(std::ostream& os) {
		os << "BST(" << length << "):{";
		for (auto itr = begin(); itr != end(); ++itr) {
			os << *itr << ", ";
		}
		os << '}' << std::endl;
	}

	void addNode(Node * el) {
		Node*after = root;
		Node*before = nullptr;
		while (after) {
			before = after;
			if (el->data > after->data) //podji desno
				after = after->right;
			else if (el->data < after->data) //podji levo
				after = after->left;
			else
				return; //ne sme isti
		}
		if (before)
			if (el->data > before->data)
				before->right = el;
			else
				before->left = el;
		else
			root = el;
		length++;

		lastAccessed.parent = before;
		lastAccessed.el = el;
	}
	Node* getPrev(Node * el)const {
		if (lastAccessed.el == el)
			return lastAccessed.parent;

		Node*after = root;
		Node*before = nullptr;
		while (after) {
			Node*preAfter = after;
			if (el->data > after->data) //podji desno
				after = after->right;
			else if (el->data < after->data) //podji levo
				after = after->left;
			else //pronadjen el!
				break;
			before = preAfter;
		}
		if (before && after->data == el->data)
			return before;
		else
			//ERROR
			return nullptr;
	}
	void removeNode(Node *el) { removeNode(getPrev(el), el); }
	void removeNode(Node * prev, Node * el) {
		Node* toSwap = nullptr;
		if (!el->left)
			toSwap = el->right;
		else if (!el->right)
			toSwap = el->left;
		else {
			Node * parent = el;
			Node * succ = parent->right;
			Node * itr = succ->left;

			while (itr) { //trazi sledbenika
				parent = succ;
				succ = itr;
				itr = itr->left;
			}
			if (parent != el) {
				parent->left = succ->right;
				succ->right = el->right;
			}
			succ->left = el->left;
			toSwap = succ;
		}
		if (!prev)
			root = toSwap;
		else if (prev->left == el)
			prev->left = toSwap;
		else
			prev->right = toSwap;
		delete el;
		length--;
	}
	Node* findNode(const T& el) const{
		if (lastAccessed.el && lastAccessed.el->data == el)
			return lastAccessed.el;

		Node*itr = root;
		lastAccessed.parent = nullptr;
		while (itr && itr->data!=el) {
			lastAccessed.parent = itr;
			if (el > itr->data) 
				itr = itr->right;
			else if (el < itr->data) 
				itr = itr->left;
		}
		return lastAccessed.el = itr;
	}
public:
	BST() = default;
	BST(const BST& b) { kopiraj(b); }
	BST(BST && b) { premesti(b); }
	BST(std::initializer_list<T> lst):BST() {
		for (auto el : lst)
			add(el);
	}
	~BST() { brisi(); }

	BST& operator=(const BST&b) {
		if (this != &b) {
			brisi();
			kopiraj(b);
		}
		return *this;
	}

	void add(const T& el) { addNode(new Node(el)); }
	T pop() {
		if (!root) return T(); //ERROR
		Node*itr = root;
		Node*el = nullptr;
		Node*prev = nullptr;

		while (itr) {
			prev = el;
			el = itr;
			itr = itr->right;
		}

		T data = el->data;
		removeNode(prev, el);
		return data;
	}
	bool has(const T& el) const { return findNode(el) ? 1 : 0; }
	void remove(const T& el) { removeNode(findNode(el)); }
	T fetch(unsigned int id){
		if (id >= length) return T(); //ERROR

		int i = 0;
		for (auto itr = begin(); itr != end(); ++itr)
			if (i++ == id)
				return *itr;
		return T(); // justincase
	}

	void forEach(void(*fn)(T& el)) {
		for (auto itr = begin(); itr != end(); ++itr) {
			fn(*itr);
		}
	}

	BST_iter begin() { return BST_iter(root); }
	BST_iter end() { return  BST_iter(nullptr); }
};