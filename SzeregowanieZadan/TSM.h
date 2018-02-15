#pragma once


#include "utils.h"

struct Move
{
	int fromIndex, toIndex;
};

// Wykorzystuje metode Tabu Search do rozwiazania problemu szeregowania zadan dla danego zbioru zadan
// Parametry:
// numOfIterations - ilosc iteracji, czas wykonywania
// tabuIterToNew - kryt dywersyfikacji, liczba iteracji bez poprawy
// tabuLength - dlug tabuList
string TSM(vector <Task*> & problemData, int numOfIterations, int tabuIterToNew, int tabuLength, bool print);

// Generuje pierwsze rozwiazanie dla metody tabu Search dla danego zbioru zadan
// Na podstawie:
//http://www.lifl.fr/~derbel/ueOC/4/refs/BilKurKir07.pdf
//WSPT, where the jobs are sequenced such that w1/c1 > w2/c2 > w3/c3 ...
vector <Task*> genFirst(vector <Task*> problemData);

// Zwraca zbiór mo¿liwych ruchów typu swap dla danej kolejnosci zadan 
vector<Move> getPossibleMoves(vector <Task*> problemData);

// Zwraca sasiada uzyskanego na podstawie zamiany danej kolejnosci ruchem swap
vector<Task*> getNeighbour(vector <Task*> problemData, int swap1, int swap2);