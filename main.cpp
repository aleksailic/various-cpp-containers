#include <iostream>
#include "List.h"
#include "SList.h"
#include "BST.h"
#include "Skup.h"
#include "PerformanceCalculator.h"

#include <random>
#undef max

using namespace std;

typedef Skup<int, BST<int>> BST_Skup;
typedef Skup<int> List_Skup;

struct perf {
	int vel;
	double formiranje;
	double umetanje;
	double brisanje;
	double pretraga;
	double operacija;

	bool operator==(const perf& rhs) { return vel == rhs.vel; }
};

void printPerf(List<perf>& pf) {
	cout << endl << "velicina\t";   pf.forEach([](perf& p) {cout << p.vel << "\t\t"; });
	cout << endl << "formiranje\t"; pf.forEach([](perf& p) {cout << p.formiranje << '\t'; });
	cout << endl << "umetanje\t";   pf.forEach([](perf& p) {cout << p.umetanje << '\t'; });
	cout << endl << "brisanje\t";   pf.forEach([](perf& p) {cout << p.brisanje << '\t'; });
	cout << endl << "pretraga\t";   pf.forEach([](perf& p) {cout << p.pretraga << '\t'; });
	cout << endl << "operacija\t";  pf.forEach([](perf& p) {cout << p.operacija << "\t\t"; });
	cout << endl;
}

int generateRandom(int low=-1000000, int high= 1000000) {
	std::random_device seed;
	std::mt19937 gen(seed());
	std::uniform_int_distribution<int> dist(low, high);

	return dist(gen);
}

int * generateRandomArr(int size) {
	int*randNiz = new int[size];
	for (int i = 0; i < size; i++)
		randNiz[i] = generateRandom();

	return randNiz;
}
void getPerformanceData() {
	PerformanceCalculator pc;
	
		//int vel_arr[]={ 200,500,1000,10000,100000,1000000 };
	int vel_arr[] = { 20,500,1000,10000 };

	List<perf> lst_valList;
	List<perf> bst_valList;

	for (int vel:vel_arr) {
		perf lst_perf,bst_perf;
		lst_perf.vel = bst_perf.vel =vel;
		double lst_vals[3];
		double bst_vals[3];
		int * randNiz = nullptr;
		
		List_Skup lSkup;
		BST_Skup bSkup;

		// FORMIRANJE
		for (int j = 0; j < 3; j++) {
			randNiz = generateRandomArr(vel);
			bSkup.erase();
			lSkup.erase();

			pc.start();
			for (int i = 0; i < vel; i++)
				lSkup.add(randNiz[i]);
			pc.stop();
			lst_vals[j] = pc.elapsedMillis();

			pc.start();
			for (int i = 0; i < vel; i++)
				bSkup.add(randNiz[i]);
				
			pc.stop();
			bst_vals[j] = pc.elapsedMillis();

			delete[] randNiz;
		}
		lst_perf.formiranje = (lst_vals[0] + lst_vals[1] + lst_vals[2]) / 3;
		bst_perf.formiranje = (bst_vals[0] + bst_vals[1] + bst_vals[2]) / 3;

		// UMETANJE
		randNiz = generateRandomArr(3);
		for (int j = 0; j < 3; j++) {
			pc.start();
			lSkup.add(randNiz[j]);
			pc.stop();
			lst_vals[j] = pc.elapsedMillis();

			pc.start();
			bSkup.add(randNiz[j]);
			pc.stop();

			bst_vals[j] = pc.elapsedMillis();
		}
		lst_perf.umetanje = (lst_vals[0] + lst_vals[1] + lst_vals[2]) / 3;
		bst_perf.umetanje = (bst_vals[0] + bst_vals[1] + bst_vals[2]) / 3;
		delete[] randNiz;

		// BRISANJE
		for (int j = 0; j < 3; j++) {
			int rnd = generateRandom(0, vel - 1);
			rnd = lSkup[rnd]; //ovako je element sigurno u skupu

			pc.start();
			lSkup.remove(rnd);
			pc.stop();
			lst_vals[j] = pc.elapsedMillis();

			pc.start();
			bSkup.add(rnd);
			pc.stop();
			bst_vals[j] = pc.elapsedMillis();
		}
		lst_perf.brisanje = (lst_vals[0] + lst_vals[1] + lst_vals[2]) / 3;
		bst_perf.brisanje = (bst_vals[0] + bst_vals[1] + bst_vals[2]) / 3;

		// PRETRAGA
		randNiz = generateRandomArr(3);
		for (int j = 0; j < 3; j++) {
			pc.start();
			lSkup.has(randNiz[j]);
			pc.stop();
			lst_vals[j] = pc.elapsedMillis();

			pc.start();
			bSkup.has(randNiz[j]);
			pc.stop();

			bst_vals[j] = pc.elapsedMillis();
		}
		lst_perf.pretraga = (lst_vals[0] + lst_vals[1] + lst_vals[2]) / 3;
		bst_perf.pretraga = (bst_vals[0] + bst_vals[1] + bst_vals[2]) / 3;
		delete[] randNiz;

		// OPERACIJA
		randNiz = generateRandomArr(vel);
		for (int j = 0; j < 3; j++) {
			List_Skup lSkup2(lSkup);
			BST_Skup bSkup2(bSkup);
			for (int i = 0; i < vel; i++) {
				bSkup2.add(randNiz[i]);
				lSkup2.add(randNiz[i]);
			}

			pc.start();
			List_Skup& lSkup3 = lSkup / lSkup2;
			pc.stop();
			delete &lSkup3;
			lst_vals[j] = pc.elapsedMillis();

			pc.start();
			BST_Skup& bSkup3 = bSkup / bSkup2;
			pc.stop();
			delete &bSkup3;
			bst_vals[j] = pc.elapsedMillis();
		}
		lst_perf.operacija = (lst_vals[0] + lst_vals[1] + lst_vals[2]) / 3;
		bst_perf.operacija = (bst_vals[0] + bst_vals[1] + bst_vals[2]) / 3;
		delete[] randNiz;

		lst_valList.add(lst_perf);
		bst_valList.add(bst_perf);
	}
	
	cout << endl<< "Procena BSTSkupa";
	printPerf(bst_valList);

	cout << endl<< "Procena ListSkupa";
	printPerf(lst_valList);
}

int main(int argc, int **argv) {
	/*
	List<int> l{ 20,10,50,40,30 };
	SList<int> sl(l);

	l += 34;
	l += 55;
	sl += 34;
	sl += 55;

	cout << l;
	cout << sl;

	BST<int> b{ 16,11,19,5,12,17,29,6,21,26,25,27 };
	cout << b << endl;
	b.remove(19);

	cout << b;

	BST_Skup sBST1(new BST<int>(b));
	BST_Skup sBST2(new BST<int>{5,12,98});

	BST_Skup& sBST3=sBST1/sBST2;
	BST_Skup& sBST4 = sBST1+sBST2;
	BST_Skup& sBST5 = BST_Skup::presek(sBST1, sBST2);

	cout << "razlika skupova\nsBST1: \t"<<sBST1 << "sBST2: \t"<<sBST2<<"je: \t"<<sBST3<<endl;
	cout << "unija skupova\nsBST1: \t" << sBST1 << "sBST2: \t" << sBST2 << "je: \t" << sBST4<<endl;
	cout << "presek skupova\nsBST1: \t" << sBST1 << "sBST2: \t" << sBST2 << "je: \t" << sBST5;

	delete &sBST3;
	delete &sBST4;
	delete &sBST5;
	*/
	getPerformanceData();

	system("pause");
	return 0;
}