#pragma once
#include "Container.h"
#include "SList.h"

template <class T, class C=SList<T>>
class Skup :public Container<T>{
private:
	C* data;
	void kopiraj(const Skup&s) {
		*data = *(s.data);
		length = s.getLength();
	}
	void brisi() {
		delete data;
		data = new C;
	}
	void premesti(Skup&s) {
		data = s.data;
		s.data = nullptr;
		length = s.getLength();
	}
	void updateLength() { length = data->getLength(); }
public:
	Skup() { 
		data = new C; 
	}
	Skup(C* cdata) :data(cdata) { updateLength(); }
	~Skup() { brisi(); }

	Skup(const Skup& s):Skup() { kopiraj(s); }
	Skup(Skup &&s):Skup() { premesti(s); }

	void add(const T& el) { data->add(el); updateLength(); }
	void remove(const T&el) { data->remove(el); updateLength(); }
	void removeRange(const T& low, const T& high) {
		for (T itr = low; itr < high; itr++) {
			data->remove(itr);
			length--;
		}
	}
	T pop() { return data->pop(); }
	T fetch(unsigned int id){ return data->fetch(id); }
	bool has(const T& el) const { return data->has(el); }
	void forEach(void(*fn)(T& el)) { data->forEach(fn); }

	static Skup& razlika(const Skup&lhs, const Skup& rhs) {
		Skup* toReturn = new Skup(lhs);
		for (auto itr = rhs.data->begin(); itr != rhs.data->end(); ++itr)
			if (toReturn->has(*itr))
				toReturn->remove(*itr);
			else
				toReturn->add(*itr);

		return *toReturn;
	}
	static Skup& unija(const Skup&lhs, const Skup& rhs) {
		Skup* toReturn = new Skup(lhs);
		for (auto itr = rhs.data->begin(); itr != rhs.data->end(); ++itr)
			toReturn->add(*itr);
		return *toReturn;
	}
	static Skup& presek(const Skup&lhs, const Skup&rhs) {
		Skup* toReturn = new Skup();
		for (auto itr = rhs.data->begin(); itr != rhs.data->end(); ++itr)
			if (lhs.has(*itr))
				toReturn->add(*itr);
		return *toReturn;
	}

	friend Skup& operator/(const Skup& lhs, const Skup& rhs) { return Skup::razlika(lhs, rhs); }
	friend Skup& operator+(const Skup& lhs, const Skup& rhs) { return Skup::unija(lhs, rhs); }


	void print(std::ostream& os) {
		os << "Skup(" << length << "):{";
		for (auto itr = data->begin(); itr != data->end(); ++itr) {
			os << *itr << ", ";
		}
		os << '}' << std::endl;
	}

};
