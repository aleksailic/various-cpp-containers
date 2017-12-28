#include <iostream>
#include <iomanip>
#include <fstream>

#include "List.h"
#include "SList.h"
#include "BST.h"
#include "Skup.h"
#include "Benchmark.h"

#ifdef max
#undef max
#endif

#define BUFFER_SIZE 1000

const unsigned long Benchmark::seed = 879521352;
SList<int> Benchmark::vel_lst={50,200,1000,10000,100000,1000000 };
int Benchmark::samples = 3;

namespace IO {
	static std::istream * input = &std::cin;
	static std::ostream * output = &std::cout;
	enum command { STDIN = 1, EXTERN_FILE };

	void close() {
		std::ifstream* fin = dynamic_cast<std::ifstream*>(input);
		if (fin && fin->is_open()) {
			fin->close();
			delete input;
			input = &std::cin;
		}
		std::ofstream* fout = dynamic_cast<std::ofstream*>(output);
		if (fout && fout->is_open()) {
			fout->close();
			delete output;
			output = &std::cout;
		}
	}
	void flushInput() {
		input->sync();
		input->clear();
		input->ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	void setInput(command id) {
		close();
		if (id == STDIN)
			input = &std::cin;
		else if (id == EXTERN_FILE) {
			char filename[100];
			std::cout << "-ime fajla: ";
			std::cin >> filename;

			input = new std::ifstream(filename);
			if (!static_cast<std::ifstream*>(input)->is_open()) throw Exception("File not opened!");
		}
	}
	void setOutput(command id) {
		close();
		if (id == STDIN)
			output = &std::cout;
		else if (id == EXTERN_FILE) {
			char filename[100];
			std::cout << "-ime fajla: ";
			std::cin >> filename;

			output = new std::ofstream(filename);
			if (!static_cast<std::ofstream*>(output)->is_open()) throw Exception("File not opened!");
		}
	}
}
namespace UI {
	enum main_command { MAKE_SET = 1, ADD_ELS, PRINT_SET, FIND_EL, INSERT_EL, REMOVE_EL, REMOVE_RANGE, GET_LEN, OP_DIFF, BENCH, END, ERR = 99 };
	enum set_command { BST_SET = 1, LIST_SET };
	enum menuid { MAIN_MENU, SET_MENU, IO_MENU };

	static menuid lastPrint = MAIN_MENU;
	static bool isActive = true;
	static bool isInitialized = false;

	void printMenu(menuid id = MAIN_MENU) {
		if (id == MAIN_MENU) {
			std::cout << "---           Domaci zadatak iz ASP-a 17/18           ---\n";
			std::cout << "- 1: Napravi skup                                       -\n";
			std::cout << "- 2: Dodaj elemente skupu                               -\n";
			std::cout << "- 3: Stampaj skup                                       -\n";
			std::cout << "- 4: Pronadji element skupa                             -\n";
			std::cout << "- 5: Ubaci element u skup                               -\n";
			std::cout << "- 6: Izbrisi element iz skupa                           -\n";
			std::cout << "- 7: Izbrisi elemente u datom opsegu, npr. [10 100]     -\n";
			std::cout << "- 8: Dohvati ukupan broj elemenata u skupu              -\n";
			std::cout << "- 9: Operacija: razlika                                 -\n";
			std::cout << "- 10: BENCHMARK                                         -\n";
			std::cout << "- 11: Kraj programa :(                                  -\n";
			std::cout << "-----------------Aleksa Ilic 0037/2016-------------------\n";
		}
		else if (id == SET_MENU) {
			std::cout << "- 1: BST                                                -\n";
			std::cout << "- 2: List                                               -\n";
		}
		else if (id == IO_MENU) {
			std::cout << "- 1: stdio                                              -\n";
			std::cout << "- 2: External file                                      -\n";
		}
		lastPrint = id;
	}
	void printErr(const char * msg = "Doslo je do greske") {
		std::cout << "! " << msg << " !\n";
	}

	unsigned short getCommand(menuid id = lastPrint) {
		unsigned short cmnd;
		std::cout << "-biram: ";
		std::cin >> cmnd;

		try {
			switch (id) {
			case MAIN_MENU:
				if (cmnd > 11) throw Exception("Invalid input");
				if (!isInitialized && (cmnd != MAKE_SET && cmnd != BENCH && cmnd != END)) throw Exception("Skup nije inicijalizovan");
				break;
			case SET_MENU:
				if (cmnd > 2) throw Exception("Invalid input");
				break;
			case IO_MENU:
				if (cmnd > 2) throw Exception("Invalid input");
				break;
			default:
				throw Exception("Error in program");
			}
		}
		catch (Exception& e) {
			printErr(e.getMsg());
			return 99;
		}
		return cmnd;
	}
	void printSuccess() {
		//std::cout << "-                    Zavrseno!                          -\n";
		std::cout << "---------------------------------------------------------\n";
	}
};


inline bool isBST(Container<int> * container) {
	if (dynamic_cast<BST_Skup*>(container))
		return true;
	else return false;
}
inline bool isLST(Container<int> * container) {
	if (dynamic_cast<List_Skup*>(container))
		return true;
	else return false;
}
void addData(Container<int> * container) {
	using IO::input;
	using IO::flushInput;

	while (!input->eof() && input->good()) {
		int elem;
		(*input) >> elem;
		if (input->bad()) break;
		if (!container) throw Exception("Skup nije inicijalizovan!");
		container->add(elem);
	}
	flushInput();
}

int main(int argc, int **argv) {
	Container<int> * container = nullptr;
	Container<int> * B = nullptr;
	Container<int> * C = nullptr;
	Benchmark * BST_bench = nullptr;
	Benchmark * List_bench = nullptr;
	int el;

	using namespace UI;
	using IO::output;
	using IO::input;

	printMenu(MAIN_MENU);
	while (isActive) {
		switch ( getCommand(MAIN_MENU) ) {
			case MAKE_SET:
				if (container)
					delete container;
				printMenu(SET_MENU);
				if ( getCommand() == BST_SET)
					container = new BST_Skup();
				else
					container = new List_Skup();
				isInitialized = true;
				break;
			case ADD_ELS:
				printMenu(IO_MENU);
				IO::setInput(static_cast<IO::command>(getCommand()));
				addData(container);
				IO::close();
				break;
			case PRINT_SET:
				printMenu(IO_MENU);
				IO::setOutput(static_cast<IO::command>(getCommand()));
				*output << *container;
				break;
			case INSERT_EL:
				*input >> el;
				container->add(el);
				break;
			case REMOVE_EL:
				*input >> el;
				container->remove(el);
				break;
			case REMOVE_RANGE:
				int low,high;
				*input >> low >> high;
				container->removeRange(low,high);
				break;
			case FIND_EL:
				*input >> el;
				if (container->has(el))
					*output << "-                 Element pronadjen!                    -\n";
				else
					*output << "-             Element nije pronadjen :(                 -\n";
				break;
			case GET_LEN:
				*output << "- Skup sadrzi: " << container->getLength() << " elemenata\n";
				break;
			case BENCH:
				BST_bench = new Benchmark(new BST_Skup);
				BST_bench->calc();
				*output << "BST BENCH:\n" << *BST_bench << "\n\n";
				delete BST_bench;

				List_bench = new Benchmark(new List_Skup);				
				List_bench->calc();
				*output << "LIST BENCH:\n" << *List_bench << "\n\n";
				delete List_bench;

				break;
			case OP_DIFF:
				if (isBST(container)) {
					B = new BST_Skup;				
				}else if (isLST(container)) {
					B = new List_Skup;
				}else {
					throw Exception("Nepoznati skupovni objekat");
					break;
				}

				std::cout << "- Demonstracija u obliku A:=A\B. Unesite elemente skupa B: \n";
				printMenu(IO_MENU);
				IO::setInput(static_cast<IO::command>(getCommand()));
				addData(B);
				IO::close(); 

				if (isBST(container))
					C = &BST_Skup::razlika(*dynamic_cast<BST_Skup*>(container), *dynamic_cast<BST_Skup*>(B));
				else
					C = &List_Skup::razlika(*dynamic_cast<List_Skup*>(container), *dynamic_cast<List_Skup*>(B));
				
				delete container;
				container = C;
				delete B;

				break;
			case ERR:
				printErr();
				IO::flushInput();
				break;
			default:
				isActive = false;
				break;
		}
		printSuccess();
	}
	delete container;
	return 0;
}