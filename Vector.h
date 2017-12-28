#pragma once
#include "Container.h"

#define CAP 100
#define MULTIPLIER 5

template <class T>
class Vector :public Container<T>
{
protected:
	unsigned int capacity;
	T * data;

	void realloc() {
		T * newData = new T[capacity];
		for (unsigned int i = 0; i < length; i++)
			newData[i] = data[i];
		delete[] data;

		data = newData;
	}
	void alloc() {
		if (data)
			delete[] data;
		data = new T[capacity];
	}
	virtual void removeElement(T*el) {
		unsigned int index = static_cast<int>(el - data);
		for (unsigned int i = index + 1; i < length; i++)
			data[i - 1] = data[i];
		length--;	
	}

	class V_iter:public Iter<T> {
		T* el;
		Vector<T>* vec;

		void kopiraj(const Iter<T>& rhs) {
			const V_iter&vitr = dynamic_cast<const V_iter&>(rhs);
			el = vitr.el;
			vec = vitr.vec;
		}
	public:
		V_iter(T*cel, Vector<T>*cvec) :el(cel),vec(cvec) {}
		V_iter(const V_iter& itr) { kopiraj(itr); }

		Iter<T>& next() {
			el++;
			return *this;
		}

		bool operator==(const Iter<T>& rhs) { 
			const V_iter& vitr = dynamic_cast<const V_iter&>(rhs);
			return el == vitr.el ? 1 : 0; 
		}
		T* operator->() { return el; }
		Iter<T>& operator=(const Iter<T>& rhs) {
			if (this != &rhs) {
				brisi();
				kopiraj(rhs);
			}
			return *this;
		}
		void operator~() {
			vec->removeElement(el);
		}
	};

	void brisi() {
		if (data) {
			delete[] data;
			data = nullptr;
		}	
	}
	void kopiraj(const Container&c) {
		const Vector& vec = dynamic_cast<const Vector&>(c);
		capacity = vec.capacity;
		data = new T[vec.size()];
		for (unsigned int i = 0; i < vec.size(); i++)
			data[i] = vec.data[i];
		length = vec.length;
	}
	void premesti(Container&&c) {
		Vector& vec = dynamic_cast<Vector&>(c);
		data = vec.data;
		capacity = vec.capacity;
		length = vec.length;
		vec.data = nullptr;
	}

	virtual void print(std::ostream& os) {
		os << "Vector" << "(" << length << "):{";
		for (auto itr = begin(); itr != end(); ++itr) {
			os << *itr << ", ";
		}
		os << '}' << std::endl;
	}
public:
	Vector(const Vector&s) { kopiraj(s); }
	Vector(Vector&&s) { premesti(s); }
	Vector(unsigned int csize) :capacity(csize), data(nullptr) {
		alloc();
	}
	Vector() :Vector(CAP) {}
	Vector(std::initializer_list<T> lst):Vector() { 
		if (lst.size() > capacity) {
			while (lst.size() > capacity)
				capacity *= MULTIPLIER;
			alloc();
		}
		for (auto el : lst)
			add(el);
	}
	~Vector() { brisi(); }

	V_iter begin() {
		return V_iter(data,this);
	}
	V_iter end() {
		return V_iter(&data[length],this);
	}

	unsigned int size() const { return capacity; }
	virtual void add(const T& el) {
		if (length >= capacity) {
			capacity *= MULTIPLIER;
			realloc();
		}
		data[length++]=el;
	}
	virtual T pop() {
		if (length)
			return data[--length];
		else
			throw Exception("Vektor je prazan, ne moze pop");
			return T();			
	}
	virtual T fetch(unsigned int index) {
		if (index >= length) {
			throw Exception("Pristup elementu koji nije u nizu");
			return T();
		}else {
			return data[index];
		}
	}
	virtual bool has(const T& el)const {
		for (unsigned int i = 0; i < length; i++)
			if (data[i] == el)
				return true;
		return false;
	}
	virtual void remove(const T& el) {
		for (auto itr = begin(); itr != end(); ++itr)
			if (*itr == el) {
				~itr;
				return;
			}
		throw Exception("Element nije pronadjen");
	}

	Vector& operator=(const Vector&c) {
		if (this != &c) {
			brisi();
			kopiraj(c);
		}
		return *this;
	}

};