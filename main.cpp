#include <iostream>
/*
template<class T>
class Iter {
private:
	virtual Iter& next() = 0;

public:
	Iter()=default;
	virtual ~Iter() {};

	Iter& operator++() { next(); }
	virtual Iter& begin() = 0;
	virtual Iter& end() = 0;
	
	virtual T* operator->() = 0;
	T& operator*(){ *operator->(); }
};*/

template <class T>
class Container {
	virtual void kopiraj(const Container& c) {}
	virtual void premesti(Container&& c) {}
	virtual void brisi() {}
	virtual void print(std::ostream& os){}
protected:
	unsigned int length;
public:
	Container() :length() {}
	Container(const Container& c) :Container() { kopiraj(c); }
	Container(Container && c) :Container() { premesti(c); }
	virtual ~Container(){ brisi(); }

	int getLength() { return length; }
	bool empty() { return length ? 1 : 0; }

	virtual void append(const T& el) = 0; //stavi na kraj
	virtual void prepend(const T& el) = 0; //stavi na pocetak
	virtual T pop() = 0; //uzmi s kraja
	virtual T fetch(unsigned int) = 0; //uzmi element po nekom redu
	virtual bool has(const T& el) = 0; // da li lista poseduje element
	virtual void remove(const T& el) = 0; //brise element T
	void erase() { brisi(); } //pozvace virtuelnu i samo ce se srediti!

	virtual void forEach(void(*fn)(T& el)) = 0; //za svaki element pozovi funkciju fn

	Container& operator=(const Container&c) {
		if (this != &c) {
			brisi();
			kopiraj(c);
		}
		return *this;
	}
	Container& operator=(Container && c) {
		if (this != &c) {
			brisi();
			premesti(c);
		}
		return *this;
	}
	Container& operator+=(const T& el) { append(el); return *this; }
	Container& operator--() { pop(); return *this; }
	T operator[](unsigned int i) {
		return fetch(i);
	}
	friend std::ostream& operator<<(std::ostream& os, Container& c) {
		c.print(os);
		return os;
	}
};


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
		for (Node*itr = front; itr != nullptr; itr = itr->nxt)
			if (itr->data == el)
				return itr;
		return nullptr;
	}
	Node* getNodeById(unsigned int id) {
		Node*itr = front;
		for (unsigned int i = 0; i < id; i++)
			itr = itr ? itr->nxt : nullptr;
		return itr;
	}
private:
	virtual void kopiraj(const Container&c) {
		const List& l = dynamic_cast<const List&>(c);
		for (Node* itr = l.front; itr != nullptr; itr = itr->nxt)
			append(itr->data);
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
		os << "List(" << length << "):{";
		forEach([](T& data) { //TO DO: CHANGE THIS!!
			std::cout << data << ", ";
		});
		os << '}' << std::endl;
	}
public:
	List() = default;
	List(std::initializer_list<T> lst) { //Lakse se je inicijalizovati inicijalizatorskom listom
		for (auto el : lst)
			append(el);
	}
	List(const List&l) { kopiraj(l); }
	List(List && l) { premesti(l); }

	virtual ~List() { brisi(); }//preuzmi destruktor

	void append(const T& el) {
		Node*nd = new Node(el);
		insertNode(rear, nd);
	}
	void prepend(const T&el) {
		Node*nd = new Node(el);
		insertNode(nullptr, nd);
	}
	T pop() {
		T data = rear->data;
		removeNode(rear);
		return data;
	}
	void remove(const T&data) { removeNode(findNode(data)); }
	bool has(const T& data) { return findNode(data) ? 1 : 0; }
	T fetch(unsigned int id) {
		if (id >= length) return T();
		Node * el = getNodeById(id);
		return el ? el->data : T();
	}

	void forEach(void(*fn)(T& data)) {
		for (Node*itr = front; itr != nullptr; itr = itr->nxt)
			fn(itr->data);
	}
};

template<class T>
class Stack : List<T> {
private:
public:
	using List::getLength;
	using List::pop;
	using List::erase;
	using List::empty;
	void push(const T& el) { append(el); }

	Stack(const Stack&s){kopiraj(s);}
	Stack(Stack&&s) { premesti(s); }
	Stack() = default;
	Stack(std::initializer_list<T> lst) { for (auto el : lst) append(el); }
};



template<class T>
class BST_Iter :public Iter {
private:
	Stack<BST<T>::Node*> stack;
public:


};

template<class T>
class BST {
protected:
	struct Node {
		Node*left;
		Node*right;
		T data;

		Node() = delete;
		Node(T& cdata) :data(cdata), left(nullptr), right(nullptr) {}
	};
	Node*root;

	class Iter {
		Stack<Node*> s;
		Node* curr;
		Node* nxt;

		void kopiraj(const Iter& rhs) {
			curr = rhs.curr;
			nxt = rhs.nxt;
			s = rhs.s;
		}
		void brisi() {
			s.erase();
		}
	public:
		Iter() = default;
		Iter(Node*start) :nxt(start) {}
		Iter(const Iter& itr) { kopiraj(itr); }
		Iter& operator++() {
			while (nxt) { //idi skroz levo dok mozes
				s.push(nxt);
				nxt = nxt->left;
			}
			if (!s.empty()) {
				nxt = s.pop();
				curr = nxt;
				nxt = nxt->right;
			}else
				curr=nullptr //dosli smo do kraja
			return *this;
		}
		bool operator==(const Iter& rhs) { return curr == rhs.curr ? 1 : 0; }
		T* operator->() { return &(curr->data); }
		T& operator*() { return *operator->(); }
		Iter& operator=(const Iter& rhs) {
			if (this != &rhs) {
				brisi();
				kopiraj(rhs);
			}
			return *this;
		}
	};

	Iter begin() { return Iter(root); }
	Iter end() { return Iter(nullptr); }

	void traverse(void(*fn)(Node**nd)) {
	}
private:
	virtual void kopiraj(const Container&c) {
		traverse([](Node**nd) {
			append(*nd);
		});
	}
	virtual void brisi() {
		traverse([](Node**nd) {
			delete *nd;
			*nd = nullptr;
		})
		root = nullptr;
	}
	virtual void premesti(Container&&c) {
		BST& b = dynamic_cast<BST&>(c);
		root = b.root;
		b.root = nullptr;
	}
	virtual void print(std::ostream& os) {
		os << "BST(" << length << "):{";
		traverse([](Node ** nd) { //TO DO: CHANGE THIS!!
			std::cout << (*nd)->data << ", ";
		});
		os << '}' << std::endl;
	}
public:
	BST() = default;
	BST(const BST& b) { kopiraj(b); }
	BST(BST && b) { premesti(b); }
	~BST() { brisi(); }

	void append(const T& el) {
		Node*nd = new Node(el);
		if (!root)

	}//stavi na kraj
	void prepend(const T& el) = 0; //stavi na pocetak
	void pop() = 0; //uzmi s kraja
	bool has(const T& el) = 0; // da li lista poseduje element
	void remove(const T& el) = 0; //brise element T

	void forEach(void(*fn)(T& el)){
		for (auto itr = begin(); itr != end(); ++itr) {
			fn(itr->data);
		}
	}
};





using namespace std;
int main(int argc, int **argv) {
	List<int> l{ 10,20,30,40,50 };

	Stack<int> s;

	system("pause");
	return 0;
}