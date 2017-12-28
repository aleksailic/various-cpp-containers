#pragma once
#include "List.h"
#include "Vector.h"

template<class T, class C = List<T>>
class PQ :private C {
public:
	using C::getLength;
	using C::erase;
	using C::empty;
	void enqueue(const T& el) { add(el); }
	T dequeue() {
		if (!length) {
			throw Exception("Prioritetni red je prazan");
			return T();
		}else {
			T elem = fetch(0);
			remove(elem);
			return elem;
		}
	}

	PQ(const PQ&s) { kopiraj(s); }
	PQ(PQ&&s) { premesti(s); }
	PQ() = default;
	PQ(std::initializer_list<T> lst) { for (auto el : lst) add(el); }

	PQ& operator=(const PQ&c) {
		if (this != &c) {
			brisi();
			kopiraj(c);
		}
		return *this;
	}
};