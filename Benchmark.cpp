#include "Benchmark.h"

std::ostream& operator<<(std::ostream& os, Benchmark& b) {
	os << "velicina\t";
	for (auto itr = b.benchData.begin(); itr != b.benchData.end(); ++itr)
		os << itr->vel << "\t\t";
	os << '\n';

	os << "formiranje\t";
	for (auto itr = b.benchData.begin(); itr != b.benchData.end(); ++itr)
		os << std::fixed << itr->formation << '\t';
	os << '\n';

	os << "umetanje\t";
	for (auto itr = b.benchData.begin(); itr != b.benchData.end(); ++itr)
		os << std::fixed << itr->insertion << '\t';
	os << '\n';

	os << "brisanje\t";
	for (auto itr = b.benchData.begin(); itr != b.benchData.end(); ++itr)
		os << std::fixed << itr->deletion << '\t';
	os << '\n';

	os << "pretraga\t";
	for (auto itr = b.benchData.begin(); itr != b.benchData.end(); ++itr)
		os << std::fixed << itr->search << '\t';
	os << '\n';

	os << "operacija\t";
	for (auto itr = b.benchData.begin(); itr != b.benchData.end(); ++itr)
		os << std::fixed << itr->operation << '\t';
	os << '\n';
	return os;
}