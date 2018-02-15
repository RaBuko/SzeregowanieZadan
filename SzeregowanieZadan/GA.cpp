#include "GA.h"


string GA(vector <Task*> & problemData, int populationCount, int numberOfIterations, double percentChanceOfMutation, bool print)
{
	string toReturn = "";
	// generowanie populacji startowej
	
	vector <vector <Task*>> population;
	vector <vector <Task*>> nextGeneration;
	pair <vector<Task*>, vector<Task*>> children;

	population.push_back(genFirstOrder(problemData));
	for (size_t i = 1; i < populationCount; i++)
		population.push_back(shuffleOrder(problemData));

	// uczynienie pierwszego obiektu z populacji startowej najlepszym
	vector <Task*> bestGlobalOrder = population[0];
	int bestGlobalCost = calculateCost(bestGlobalOrder);

	

	while (numberOfIterations > 0)
	{
		nextGeneration = population;
		if (print)
		{
			toReturn += "\n------------------------------------------------------------\nIlosc pozostalych iteracji = " + to_string(numberOfIterations) + "\n";
			toReturn += "BestGlobalOrder: " + orderToString(bestGlobalOrder) + "\n";
			toReturn += "BestGlobalCost : " + to_string(bestGlobalCost) + "\n";
			toReturn += "PopulationCount: " + to_string(nextGeneration.size()) + "\n";
		}
		
		// krzyzowanie osobnikow ze soba i twrzenie nowej generacji
		for (size_t i = 0; i < populationCount; i+=2)
		{
			children = crossover(population[i], population[i + 1]);
			nextGeneration.push_back(children.first);
			nextGeneration.push_back(children.second);
			if (print)
			{
				toReturn += "Rodzic " + to_string(i) + " : " + orderToString(population[i]) + "\n";
				toReturn += "Rodzic " + to_string(i + 1) + " : " + orderToString(population[i + 1]) + "\n";
				toReturn += "Dziecko 1 : " + orderToString(children.first) + "\n";
				toReturn += "Dziecko 2 : " + orderToString(children.second) + "\n\n";
			}
		}
		if (print) toReturn += "PopulationCount: " + to_string(nextGeneration.size()) + "\n";

		// mutowanie osobnikow z nowej generacji
		for (size_t i = populationCount; i < nextGeneration.size(); i++)
		{
			int chance = rand() % 100;
			if ( chance <= percentChanceOfMutation)
			{
				if (print)
				{
					toReturn += "Mutacja dla osobnika " + to_string(i) + "\n";
					toReturn += "Przed: " + orderToString(nextGeneration[i]) + "\n";
				}
				nextGeneration[i] = mutateOrder(nextGeneration[i]);
				if (print) toReturn += "Po	  : " + orderToString(nextGeneration[i]) + "\n";
			}
		}

		// sortowanie populacji
		nextGeneration = sortPopulation(nextGeneration);
		population.clear();

		// zachowanie najlepszych osobnikow
		for (size_t i = 0; i < populationCount; i++)
		{
			population.push_back(nextGeneration[i]);
		}

		numberOfIterations--;
		bestGlobalOrder = nextGeneration[0];
		bestGlobalCost = calculateCost(bestGlobalOrder);
	}
	
	toReturn += "Najlepsza sciezka: " + orderToString(bestGlobalOrder) + "\n";
	toReturn += "Najlepszy koszt: " + to_string(bestGlobalCost) + "\n";
	return toReturn;
}

vector <vector <Task*>> sortPopulation(vector <vector <Task*>> population)
{
	int n = population.size();
	do
	{
		for (size_t i = 0; i < n - 1; i++)
		{
			if (calculateCost(population[i]) > calculateCost(population[i + 1]))
			{
				swap(population[i], population[i + 1]);
			}
		}
		n = n - 1;
	} while (n > 1);
	return population;
}

vector <Task*> mutateOrder(vector <Task*> oldOrder)
{
	vector <Task*> vec = oldOrder;
	int orderLength = oldOrder.size();
	int startIndex = rand() % orderLength;
	int endIndex = startIndex + rand() % (orderLength - startIndex);
	swap(vec[startIndex], vec[endIndex]);
	return vec;
}

pair< vector<Task*>, vector<Task*> > crossover(vector <Task*> parent1, vector <Task*> parent2)
{
	pair <vector<Task*>, vector<Task*>> children; // inicjalizacja dzieci wynikajacych z krzyzowania
	
	int orderLength = parent1.size();
	int startIndex = rand() % orderLength; // losowo wybrany index startowy sekcji dopasowania
	if (startIndex == orderLength - 1) startIndex = 0;
	int endIndex = (rand() % (orderLength - startIndex)) + startIndex + 1; // losowo wybrany index koncowy sekcji dopasowania (musi sie znajdowac po startIndex)
	//cout << "Start: " << startIndex << "| End: " << endIndex << "|Lenght: " << orderLength << endl;
	//cout << "Parent1: " << orderToString(parent1) << endl;
	//cout << "Parent2: " << orderToString(parent2) << endl;

	children.first = parent2; // dziecko pierwsze chwilo w pelni dziedziczy po rodzicu 2
	children.second = parent1; // odwrotnie z drugim dzieckiem
	
	// uczynienie indeksow poza sekcja dopasowania w dzieciach jeszcze nieznanych
	for (size_t i = 0; i < orderLength; i++)
	{
		if (i < startIndex || i >= endIndex)
		{
			children.first[i] = new Task(-1, INT_MAX, INT_MAX, INT_MAX);
			children.second[i] = new Task(-1, INT_MAX, INT_MAX, INT_MAX);
		}
	}

	// wypelnianie dzieci odpowiednimi zadaniami poza sekcja dopasowania
	for (int i = 0; i < orderLength; i++)
	{
		// pominiecie sekcji dopasowania
		if (i >= startIndex && i < endIndex)
			continue;

		// uzupelnienie niekonfliktujacych zadan z podstawiona sekcja dopasowania dla dziecka 1
		if (!isBeetweenInTask(startIndex, endIndex, children.first, parent1[i]))
			children.first[i] = parent1[i];

		// uzupelnienie niekonfliktujacych zadan z podstawiona sekcja dopasowania dla dziecka 2
		if (!isBeetweenInTask(startIndex, endIndex, children.second, parent2[i]))
			children.second[i] = parent2[i];
	}
	
	for (int i = 0; i < orderLength; i++)
	{
		if (children.first[i]->index == -1)
		{
			//cout << "C1: I : " << i << " |";
			for (int j = 0; j < orderLength; j++)
			{
				if (!isBeetweenInTask(0, orderLength, children.first, parent1[j]))
				{
					//cout << "Nie ma j = " << parent1[j]->index << " jeszcze w C1" << endl;
					children.first[i] = parent1[j];
					break;
				}
			}
		}

		if (children.second[i]->index == -1)
		{
			//cout << "C2: I : " << i << " |";
			for (size_t j = 0; j < orderLength; j++)
			{
				if (!isBeetweenInTask(0, orderLength, children.second, parent2[j]))
				{
					//cout << "Nie ma j = " << parent2[j]->index << " jeszcze w C2" << endl;
					children.second[i] = parent2[j];
					break;
				}
			}
		}
	}
	//cout << "-----------------------------------------------------------------------------------\n";
	//cout << "Child 1: " << orderToString(children.first) << endl;
	//cout << "Child 2: " << orderToString(children.second) << endl;

	return children;
}

bool isBeetweenInTask(int startIndex, int endIndex, vector <Task*> orderToCheck, Task* taskToCheck)
{
	for (int i = startIndex; i < endIndex; i++)
		if (taskToCheck == orderToCheck[i])
			return true;

	return false;
}

vector <Task*> genFirstOrder(vector <Task*> problemData)
{
	vector <Task*> best = problemData;
	vector <float> wc;
	for (size_t i = 0; i < problemData.size(); i++)
		wc.push_back((float)problemData[i]->weight / (float)problemData[i]->time);


	for (int i = 0; i < problemData.size(); i++)
	{
		for (int j = 0; j < problemData.size() - 1; j++)
		{
			if (wc[j] < wc[j + 1])
			{
				swap(best[j], best[j + 1]);
				swap(wc[j], wc[j + 1]);
			}
		}
	}
	orderToString(best);
	return best;
}
