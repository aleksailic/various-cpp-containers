#pragma once
#include "Container.h"
#include "List.h"
#include "SList.h"
#include "BST.h"
#include "Skup.h"

#include <random>
#include "PerformanceCalculator.h"

class Benchmark {
	struct perf {
		int vel;
		double formation;
		double insertion;
		double deletion;
		double search;
		double operation;

		bool operator==(const perf& rhs) { return vel == rhs.vel; }
		friend std::ostream& operator<<(std::ostream& os, const Benchmark::perf&p) {
			os << "Perf(" << p.vel << "): ";
			os << "formiranje: " << p.formation << ',';
			os << "umetanje: " << p.insertion << ',';
			os << "brisanje: " << p.deletion << ', ';
			os << "pretraga " << p.search << ', ';
			os << "operacija" << p.operation << '\n';
			return os;
		}
	};

	PerformanceCalculator pc;
	Container<int> * container;
	List<perf> benchData;
	static int samples;
	static SList<int> vel_lst;

	static const unsigned long seed;
	std::mt19937 generator;

	int generateRandom(int low = -1000000, int high = 1000000) {
		std::uniform_int_distribution<int> dist(low, high);
		return dist(generator);
	}
	int * generateRandomArr(int size) {
		int*randNiz = new int[size];
		for (int i = 0; i < size; i++)
			randNiz[i] = generateRandom();

		return randNiz;
	}

	double calcFormation(int vel) {
		double perfData = 0;
		int * randNiz;

		for (int j = 0; j < samples; j++) {
			randNiz = generateRandomArr(vel);
			container->erase();
			pc.start();
			for (int i = 0; i < vel; i++)
				container->add(randNiz[i]);
			pc.stop();
			perfData += pc.elapsedMillis();
			delete[] randNiz;
		}
		perfData /= samples;
		return perfData;
	}
	double calcInsertion(int vel) {
		double perfData = 0;
		int * randNiz = generateRandomArr(samples);

		for (int j = 0; j < samples; j++) {
			pc.start();
			container->add(randNiz[j]);
			pc.stop();
			perfData += pc.elapsedMillis();
		}

		delete[] randNiz;
		perfData /= samples;
		return perfData;
	}
	double calcDeletion(int vel) {
		double perfData = 0;
		for (int j = 0; j < samples; j++) {
			int rnd = generateRandom(0, container->getLength() - 1);
			rnd = container->fetch(rnd); //ovako je element sigurno u skupu

			pc.start();
			container->remove(rnd);
			pc.stop();
			perfData += pc.elapsedMillis();
		}
		perfData /= samples;
		return perfData;
	}
	double calcSearch(int vel) {
		int * randNiz = generateRandomArr(samples);
		double perfData = 0;
		for (int j = 0; j < samples; j++) {
			pc.start();
			container->has(randNiz[j]);
			pc.stop();
			perfData += pc.elapsedMillis();
		}
		delete[] randNiz;
		perfData /= samples;
		return perfData;
	}
	double calcOperation(int vel) {
		double perfData = 0;
		for (int j = 0; j < samples; j++) {
			Container<int> * container2;
			bool isBST = true;
			if (dynamic_cast<BST_Skup*>(container))
				container2 = new BST_Skup;
			else if (dynamic_cast<List_Skup*>(container)) {
				container2 = new List_Skup;
				isBST = false;
			}
			else
				return 0;

			int * randNiz = generateRandomArr(vel);
			for (int i = 0; i < vel; i++)
				container2->add(randNiz[i]);
			delete[] randNiz;

			Container<int> * container3;
			pc.start();
			if (isBST)
				container3 = &BST_Skup::razlika(*dynamic_cast<BST_Skup*>(container), *dynamic_cast<BST_Skup*>(container2));
			else
				container3 = &List_Skup::razlika(*dynamic_cast<List_Skup*>(container), *dynamic_cast<List_Skup*>(container2));
			pc.stop();
			perfData += pc.elapsedMillis();
			delete container3;
			delete container2;
		}
		perfData /= samples;
		return perfData;
	}

public:
	Benchmark(Container<int> * ccontainer) :container(ccontainer), generator(std::mt19937(seed)) {}
	~Benchmark() { delete container; }

	friend std::ostream& operator<<(std::ostream& os, Benchmark& b);
	void calc() {
		benchData.erase();
		for (int vel : vel_lst) {
			if (dynamic_cast<List_Skup*>(container) && vel > 100000)continue;
			perf bench;
			bench.vel = vel;
			bench.formation = calcFormation(vel);
			bench.insertion = calcInsertion(vel);
			bench.deletion = calcDeletion(vel);
			bench.search = calcSearch(vel);
			bench.operation = calcOperation(vel);

			benchData.add(bench);
			container->erase();
		}
	}
};