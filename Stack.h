#pragma once
#include "List.h"
template<class T>
class Stack :private List<T>
{
const char* name = "Stack";
public:
	using List::getLength;
	using List::pop;
	using List::erase;
	using List::empty;
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