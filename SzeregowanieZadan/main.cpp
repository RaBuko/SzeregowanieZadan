

#include "Task.h"
#include "utils.h"
#include "DPA.h"
#include "TSM.h"
#include "GA.h"

using namespace std;

//zmienne potrzebne do u¿ycia timera
double PCFreq = 0.0; 
__int64 CounterStart = 0;

// Deklaracje metod
void StartCounter(); // rozpoczêcie liczenia czasu
double GetCounter(); // zakoñczenie mierzenia czasu i wyœwietlenie
void wait();
bool questionPrint();

vector <Task*> tasks; // zbior zadan dla problemu szeregowania zadan
map <string,unsigned int> subsets; // podzbiory problemu dla algorytmu programowania dynamicznego

// Zmienne przechowuj¹ce wybory dokonnywane w menu programu
unsigned int mainMenuChoice = -1, algMenuChoice = -1, testsNumTestChoice = 0, testsNumTasksChoice = 0, testsAlgChoice = -1;

string strToReturn = "";
double timeSum = 0;
bool print = false;

int numOfIterations = 10;
// Zmienne Tabu Search
int tabuIterToNew = 3, tabuLength = 5;
// Zmienna algorymtu genetycznego
int genPopulation = 20;
double genChanceMutation = 1;
int main(int argc, char * argv[])
{
	
	srand(time(nullptr));
	while (mainMenuChoice)
	{
		strToReturn = "";
		system("cls");
		cout << "Projekt PEA\n" <<
			"Imie nazwisko\n" <<
			"Indeks: \n\n" <<
			"PROBLEM SZEREGOWANIA ZADAN NA JEDNYM PROCESORZE\n" <<
			"Co chcesz zrobic?\n" <<
			"1. Wyswietl zadania\n" <<
			"2. Wczytaj\n" <<
			"3. Zapisz do pliku\n" <<
			"4. Losuj zadania\n" <<
			"5. ROZWIAZ PROBLEM\n" <<
			"6. Wykonaj testy dla konkretnego algorytmu\n" <<
			"0. Wyjdz z programu\n" <<
			"Wybor: ";
		mainMenuChoice = _getch() - 48;
		switch (mainMenuChoice)
		{
		case 1:
			cout << "WYSWIETLANIE ZALADOWANYCH ZADAN\n\n";
			showTasks(tasks);
			wait();
			break;

		case 2:
			tasks.clear();
			cout << "WCZYTANIE DANYCH Z PLIKU\n\n";
			cout << "Poprzednie zapisane dane zosta³y usuniete\n";
			cout << "Podaj nazwe pliku: ";
			cin >> strToReturn;
			StartCounter();
			tasks = loadFromFile(strToReturn);
			cout << "Operacja wykonana w czasie " << GetCounter() << "ms\n";
			wait();
			break;

		case 3:
			cout << "ZAPISYWANIE DANYCH DO PLIKU\n\n";
			StartCounter();
			saveToFile("dane.txt", tasks);
			cout << "Operacja wykonana w czasie " << GetCounter() << "ms\n";
			wait();
			break;

		case 4:
			tasks.clear();
			cout << "LOSOWANIE ZADAN\n\n";
			cout << "Ile zadan chcesz wygenerowac? ";
			cin >> strToReturn;
			try { 
				if (stoi(strToReturn) < 1)
				{
					cout << "Za mala ilosc zadan\n";
					break;
				}
			}
			catch (invalid_argument) { cout << "Dane sa niepoprawne\n"; break; }
			StartCounter();
			generateTasks(tasks, stoi(strToReturn));
			cout << "Operacja wykonana w czasie " << GetCounter() << "ms\n";
			wait();
			break;

		case 5:
			
			algMenuChoice = -1;
			while (algMenuChoice)
			{
				cout << "ROZWIAZ PROBLEM ZA POMOCA WYBRANEGO ALGORYTMU/METODY:\n";
				cout << "1. Algorytm programowania dynamicznego\n";
				cout << "2. Metoda Tabu Search\n";
				cout << "3. Algorytm genetyczny\n";
				cout << "4. Brute Force\n";
				cout << "0. Powrot do menu glownego\n";
				cout << "Wybor: ";
				algMenuChoice = _getch() - 48;
				switch (algMenuChoice)
				{
				case 1:
					cout << "ALGORYTM PROGRAMOWANIA DYNAMICZNEGO\n\n";
					print = questionPrint();

					cout << "\nGenerowanie podzbiorow\n";
					subsets.emplace(bitset<number_of_digits>(0).to_string(), 0);
					for (size_t i = 1; i < pow(2, tasks.size()); i++)
						subsets.emplace(bitset<number_of_digits>(i).to_string(), INT_MAX);

					cout << "Rozpoczecie algorytmu\n";
					StartCounter();
					strToReturn = DPA(tasks, subsets, print);
					cout << "Operacja wykonana w czasie " << GetCounter() << "ms\n";
					subsets.clear();
					cout << strToReturn;
					wait();
					algMenuChoice = 0;
					break;
				case 2:
					cout << "METODA TABU SEARCH\n";
					print = questionPrint();

					
					cout << "Podaj ilosc iteracji (okresla czas trwania wykonywania algorytmu): ";
					cin >> numOfIterations;
					cout << "Podaj kryterium dywersyfikacji (liczba iteracji bez poprawy): ";
					cin >> tabuIterToNew;
					cout << "Podaj dlugosc listy tabu: ";
					cin >> tabuLength;

					cout << "\nAKTUALNA INSTANCJA PROBLEMU:";
					showTasks(tasks);

					cout << "Rozpoczecie algorytmu\n";
					StartCounter();
					strToReturn = TSM(tasks, numOfIterations, tabuIterToNew, tabuLength, print);
					cout << "Operacja wykonana w czasie " << GetCounter() << "ms\n";
					cout << strToReturn;

					wait();
					algMenuChoice = 0;
					break;
				case 3: 
					cout << "ALGORYTM GENETYCZNY\n\n";
					print = questionPrint();

					cout << "Podaj wielkosc populacji (musi byc parzysta): ";
					cin >> genPopulation;
					cout << "Podaj ilosc iteracji (okresla czas trwania wykonywania algorytmu): ";
					cin >> numOfIterations;
					cout << "Podaj szanse zajscia mutacji dla danego elementu populacji (w procentach, zalecane do 10%): ";
					cin >> genChanceMutation;

					StartCounter();
					strToReturn = GA(tasks, genPopulation, numOfIterations, genChanceMutation, print);
					cout << "Operacja wykonana w czasie " << GetCounter() << "ms\n";
					cout << strToReturn;

					wait();
					algMenuChoice = 0;
					break;
				case 4:
					cout << "BRUTE FORCE\n\n";
					print = questionPrint();
					StartCounter();
					strToReturn = BruteForce(tasks, print);
					cout << "Operacja wykonana w czasie " << GetCounter() << "ms\n";
					cout << strToReturn;

					wait();
					algMenuChoice = 0;
					break;
				default:
					break;
				}
			}
			break;

		case 6:
			cout << "TESTOWANIE\n\n";
			cout << "Jakim algorytmem chcesz rozwiazac problem?\n";
			cout << "1. Algorytm programowania dynamicznego\n";
			cout << "2. Metoda Tabu Search\n";
			cout << "3. Algorytm genetyczny \n";
			cout << "4. Brute Force\n";
			cout << "0. Powrot do menu glownego\n";
			cout << "Wybor : ";
			try {
				testsAlgChoice = _getch() - 48;
				if (testsAlgChoice == 1)
					cout << "\nWybrano algorytm programowania dynamicznego\n";
				else if (testsAlgChoice == 2)
					cout << "\nWybrano metode Tabu Search\n";
				else if (testsAlgChoice == 3)
					cout << "\nWybrano algorytm genetyczny\n";
				else if (testsAlgChoice == 4)
					cout << "\nWybrano BruteForce - przeglad zupelny\n";
				else
				{
					cout << "Niepoprawnie podane dane, powrot do menu glownego\n";
					break;
				}
			}
			catch (invalid_argument) { cout << "Dane sa niepoprawne\n"; wait(); break; }
			cout << "Podaj ilosc zadan dla testow: ";
			try {
				cin >> testsNumTasksChoice;
				if (testsNumTasksChoice < 1)
				{
					cout << "Za mala ilosc testow\n";
					break;
				}
			}
			catch (invalid_argument) { cout << "Dane sa niepoprawne\n"; wait(); break; }
			cout << "Podaj ilosc testow ktore chcesz wykonac: ";
			try {
				cin >> testsNumTestChoice;
				if (testsNumTestChoice < 1)
				{
					cout << "Za mala ilosc testow\n";
					break;
				}
			}
			catch (invalid_argument) { cout << "Dane sa niepoprawne\n"; wait(); break; }

			if (testsAlgChoice == 2)
			{
				cout << "Podaj ilosc iteracji (okresla czas trwania wykonywania algorytmu): ";
				cin >> numOfIterations;
				cout << "Podaj kryterium dywersyfikacji (liczba iteracji bez poprawy): ";
				cin >> tabuIterToNew;
				cout << "Podaj dlugosc listy tabu: ";
				cin >> tabuLength;
			}

			if (testsAlgChoice == 3)
			{
				cout << "Podaj wielkosc populacji (musi byc parzysta): ";
				cin >> genPopulation;
				cout << "Podaj ilosc iteracji (okresla czas trwania wykonywania algorytmu): ";
				cin >> numOfIterations;
				cout << "Podaj szanse zajscia mutacji dla danego elementu populacji (w procentach, zalecane do 10%): ";
				cin >> genChanceMutation;
			}

			timeSum = 0;
			for (size_t i = 0; i < testsNumTestChoice; i++)
			{
				tasks.clear();
				cout << setw(3) << i + 1 << "| Tworzenie zadan ";
				generateTasks(tasks, testsNumTasksChoice);
				if (testsAlgChoice == 1)
				{
					cout << "| Generowanie podzbiorow ";
					for (size_t i = 1; i < pow(2, tasks.size()); i++)
						subsets.emplace(bitset<number_of_digits>(i).to_string(), INT_MAX);
					cout << "| START ... ";
					StartCounter();
					strToReturn = DPA(tasks, subsets, false);
					timeSum += GetCounter();
					cout << "Operacja wykonana w czasie " << GetCounter() << "ms\n";
					subsets.clear();
					
				}
				if (testsAlgChoice == 2)
				{
					cout << "| START ... ";
					StartCounter();
					strToReturn = TSM(tasks, numOfIterations, tabuIterToNew, tabuLength, false);
					timeSum += GetCounter();
					cout << "Operacja wykonana w czasie " << GetCounter() << "ms\n";
				}
				if (testsAlgChoice == 3)
				{
					cout << "| START ... ";
					StartCounter();
					strToReturn = GA(tasks, genPopulation, numOfIterations, genChanceMutation, false);
					timeSum += GetCounter();
					cout << "Operacja wykonana w czasie " << GetCounter() << "ms\n";
				}
				if (testsAlgChoice == 4)
				{
					cout << "| START ... ";
					StartCounter();
					strToReturn = BruteForce(tasks, false);
					timeSum += GetCounter();
					cout << "Operacja wykonana w czasie " << GetCounter() << "ms\n";
				}
			}
			cout << "\n\nTESTY ZAKONCZONE\n";
			cout << "Ilosc testow : " << testsNumTestChoice << "\n";
			cout << "Sredni czas wykonania operacji : " << timeSum / testsNumTestChoice << " ms\n";
			wait();
			break;
		default:
			break;
		}
	}
}

void StartCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		cout << "QueryPerformanceFrequency failed!\n";

	PCFreq = double(li.QuadPart) / 1000.0;

	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
}

double GetCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return ((li.QuadPart - CounterStart) / PCFreq);
}

void wait()
{
	cout << "\n\nNacisnij dowolny przycisk aby kontynuowac...";
	_getch();
}

bool questionPrint()
{
	string to_return;
	cout << "Czy po wykonaniu operacji wypisac etapy algorytmu? t - Tak, n - Nie : ";
	cin >> to_return;
	if (to_return == "t" || to_return == "T")
		return true;
	else if (to_return == "n" || to_return == "N")
		return false;
	else
	{
		cout << "\nZle podana odpowiedz, wykonanie algorytmu bez wypisywania\n";
		return false;
	}
}