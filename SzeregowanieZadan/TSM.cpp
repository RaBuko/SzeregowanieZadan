#include "TSM.h"


string TSM(vector <Task*> & problemData, int numOfIterations, int tabuIterToNew, int tabuLength, bool print)
{
	string toReturn = "";
	int diversification = 0;
	bool onTabuFlag = false;

	//Tworzenie pierwszego rozwiazania
	if (print) toReturn += "Pierwsze wygenerowane rozwiazanie: ";
	vector <Task*> bestGlobal = genFirst(problemData);
	vector <Task*> bestCandidate = bestGlobal;
	int actualBestCost = calculateCost(bestGlobal);
	if (print) toReturn += orderToString(bestGlobal) + " | Koszt: " + to_string(actualBestCost) + "\n";

	// Inicjowanie tabuList
	if (print) toReturn += "Tworzenie TabuList\n";
	vector <Move> tabuList;

	while (numOfIterations > 0)
	{
		//drukowanie danych
		if (print)
		{
			toReturn +=
				"Iteracja: " + to_string(numOfIterations) + "\nPoziom dywersyfikacji (max) " + to_string(diversification) + "(" + to_string(tabuIterToNew) + ")" + "\n" +
				"  TabuList: tabuList.size = " + to_string(tabuList.size()) + "\n";

			for (size_t i = 0; i < tabuList.size(); i++)
				toReturn += to_string(i) + ": from " + to_string(tabuList[i].fromIndex) + " to " + to_string(tabuList[i].toIndex) + "\n";

			toReturn +=
				"  BestGlobal: " + orderToString(bestGlobal) + " | Koszt: " + to_string(actualBestCost) + "\n" +
				"  BestCandidate " + orderToString(bestCandidate) + " | Koszt: " + to_string(calculateCost(bestCandidate)) + "\n\n";
		}

		// sprawdzenie czy dywersyfikacja nie przekracza aktualnie maksymalnej na³ó¿onej
		if (diversification >= tabuIterToNew)
		{
			if (print) toReturn += "Generowanie nowej kolejnosci\n";
			diversification = 0;
			bestCandidate = shuffleOrder(problemData);
		}

		//Generowanie mozliwe ruchy (swapy kazdej pary)
		if (print) toReturn += "  Generowanie mozliwych ruchow dla " + orderToString(bestCandidate) + "\n";
		vector<Move> possibleMoves = getPossibleMoves(problemData);

		// Sprawdzenie sasiedztwa
		for (int i = 0; i < possibleMoves.size(); i++)
		{
			// Tworzenie nowego kandydata
			vector <Task*> neighbour = bestCandidate;
			swap(neighbour[possibleMoves[i].fromIndex], neighbour[possibleMoves[i].toIndex]);
			if (print) toReturn += "  Sasiad " + to_string(i) + " : " + orderToString(neighbour) + "\n";

			onTabuFlag = false;
			// sprawdzenie czy ruch jest w tabuLiscie i czy dzieki niemu ma lepszy koszt niz poprzedni kandydaci
			if (calculateCost(neighbour) < calculateCost(bestCandidate))
			{
				for (size_t i = 0; i < tabuList.size(); i++)
				{
					if (tabuList[i].fromIndex == possibleMoves[i].fromIndex && tabuList[i].toIndex == possibleMoves[i].toIndex)
					{
						onTabuFlag = true;
						break;
					}
				}

				if (!onTabuFlag)
				{
					if (print) toReturn += "* Nowy kandydat!\n";
					bestCandidate = neighbour;
					tabuList.push_back(possibleMoves[i]);
					onTabuFlag = false;
				}
			}
		}

		// sprawdzenie czy nie przekroczono wielkosci tabuList
		if (tabuList.size() > tabuLength)
		{
			if (print) toReturn += "Przekroczenie wielkosci tabuList elementu z TabuList, usuwanie elementu z przodu\n";
			tabuList.erase(tabuList.begin());
		}

		// Sprawdzenie czy koszt aktualnego kadnydata jest lepszy niz globalnego, wtedy on sie staje globalnym
		if (calculateCost(bestCandidate) < actualBestCost)
		{
			if (print) toReturn += "\n**Nowe GLOBALNE rozwiazanie\n";
			bestGlobal = bestCandidate;
			actualBestCost = calculateCost(bestGlobal);
			diversification = 0;
		}
		else
			diversification += 1;

		if (print) toReturn += "\n\n";
		numOfIterations--;
	}

	return toReturn += "\n\nNAJLEPSZE ROZWIAZANIE GLOBALNE = " + to_string(actualBestCost);
}

vector <Task*> genFirst(vector <Task*> problemData)
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

vector<Move> getPossibleMoves(vector <Task*> problemData)
{
	vector <Move> possibleMoves;
	Move move;
	int i, j;
	for (i = 0; i < problemData.size() - 1; i++)
	{
		for (j = i + 1; j < problemData.size(); j++)
		{
			move.fromIndex = i;
			move.toIndex = j;
			possibleMoves.push_back(move);
		}
	}
	return possibleMoves;

}

vector<Task*> getNeighbour(vector <Task*> problemData, int swap1, int swap2)
{
	vector<Task*> candidate = problemData;
	swap(candidate[swap1], candidate[swap2]);
	return candidate;
}