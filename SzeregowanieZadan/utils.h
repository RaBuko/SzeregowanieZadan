#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <bitset>
#include <map>
#include <random>
#include <utility>
#include <fstream>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <math.h>
#include <string>

#include "Task.h"

using namespace std;

const int number_of_digits = 24;




// Zwraca kolejnosc zadan jako lancuch znakow
string orderToString(vector <Task*> problemData);

// Generuje nowa kolejnosc zadan na podstawie podanego zbioru zadan
vector<Task*> shuffleOrder(vector <Task*> problemData);

// Algorytm przegladu zupelnego dla problemu szeregowania zadan
string BruteForce(vector <Task*> & problem_data, bool print);

// Generuje zbior zadan dla problemu
void generateTasks(vector<Task*>& problem_data, unsigned int n);

// Wypisuje w konsoli zbior zadan z parametrami
void showTasks(vector <Task*> & data);

// Kalkuluje i zwraca sume opoznien (koszt) dla danej kolejnosci zadan
int calculateCost(vector <Task*> arrOfTasks);

// Wczytuje z podanej nazwy pliku zbior zadan dla problemu i go zwraca
vector <Task*> loadFromFile(string filename);

// Zapisuje do podanego pliku podany zbior zadan
void saveToFile(string filename, vector <Task*> & tasksToSave);