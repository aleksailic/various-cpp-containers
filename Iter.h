#pragma once
template<class T>
class Iter {
protected:
	virtual void kopiraj(const Iter& itr) {}
	virtual void brisi() {}
public:
	Iter() = default;
	Iter(const Iter& itr) { kopiraj(itr); }
	virtual ~Iter() {};

	virtual Iter& next() = 0;
	Iter& operator++() { return next(); }

	virtual T* operator->() = 0;
	T& operator*() { return *operator->(); }

	virtual bool operator==(const Iter& rhs) = 0;
	bool operator!=(const Iter& rhs) { return !operator==(rhs); }
	Iter& operator=(const Iter& rhs) {
		if (this != &rhs) {
			brisi();
			kopiraj(rhs);
		}
		return *this;
	}
};
