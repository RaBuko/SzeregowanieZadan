#pragma once
#include "utils.h"

string GA(vector <Task*> & problemData, int populationCount, int numberOfIterations, double percentChanceOfMutation, bool print);

vector<vector<Task*>> sortPopulation(vector<vector<Task*>> population);

vector <Task*> mutateOrder(vector <Task*> oldOrder);

pair< vector<Task*>, vector<Task*> > crossover(vector <Task*> parent1, vector <Task*> parent2);

bool isBeetweenInTask(int startIndex, int endIndex, vector <Task*> orderToCheck, Task* taskToCheck);

vector <Task*> genFirstOrder(vector <Task*> problemData);