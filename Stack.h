#pragma once
#include "List.h"
#include "Vector.h"

template<class T,class C= List<T>>
class Stack :private C{
public:
	using C::getLength;
	using C::pop;
	using C::erase;
	using C::empty;
	void push(const T& el) { add(el); }

	Stack(const Stack&s) { kopiraj(s); }
	Stack(Stack&&s) { premesti(s); }
	Stack() = default;
	Stack(std::initializer_list<T> lst) { for (auto el : lst) add(el); }

	Stack& operator=(const Stack&c) {
		if (this != &c) {
			brisi();
			kopiraj(c);
		}
		return *this;
	}
};