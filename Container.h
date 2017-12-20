#pragma once
#include "Iter.h"
#include <iosfwd>

template <class T>
class Container {
	virtual void kopiraj(const Container& c) {}
	virtual void premesti(Container&& c) {}
	virtual void brisi() {}
	virtual void print(std::ostream& os) {}
protected:
	unsigned int length;
public:
	Container() :length() {}
	Container(const Container& c) :Container() { kopiraj(c); }
	Container(Container && c) :Container() { premesti(c); }
	virtual ~Container() { brisi(); }

	int getLength() const { return length; }
	bool empty() const { return length>0 ? 0 : 1; }

	virtual void add(const T& el) = 0; //dodaj element
	virtual T pop() = 0; //uzmi s kraja
	virtual T fetch(unsigned int) = 0; //uzmi element po nekom redu
	virtual bool has(const T& el)const = 0; // da li lista poseduje element
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
	Container& operator+=(const T& el) { add(el); return *this; }
	Container& operator--() { pop(); return *this; }
	T operator[](unsigned int i) {
		return fetch(i);
	}
	friend std::ostream& operator<<(std::ostream& os, Container& c) {
		c.print(os);
		return os;
	}

};