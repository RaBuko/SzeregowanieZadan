#include "DPA.h"

string DPA(vector <Task*> & problem_data, map <string, unsigned int> & subsets, bool print)
{
	int rozw = INT_MAX, result = 0, cmax = 0, index = 0; //rozw, result - najlepsze rozwiazanie dla danego i (result chwilowe); index - prawidlowe wyliczenie indeksu zadania od prawej
	string tmp = "", to_return = ""; // tmp - string na chwilowe przechowanie i; to_return - string do zwrocenia po wykonaniu sie algorytmu

	for (size_t i = 1; i < subsets.size(); i++)
	{
		tmp = bitset<number_of_digits>(i).to_string(); // skopiowanie i do tmp jako liczba binarna
		rozw = INT_MAX;
		cmax = 0;
		if (print) to_return += "OPT(" + tmp + "):\n";

		for (size_t j = 0; j < tmp.length(); j++)
			if (tmp[j] == '1')
				cmax += problem_data[number_of_digits - j - 1]->time; // wyliczenie cmax calkowitego dla danego i

		if (print) to_return += "	CMAX : " + to_string(cmax) + "\n";
		for (size_t j = 0; j < tmp.length(); j++)
		{
			if (tmp[j] == '1')
			{
				index = number_of_digits - j - 1;
				tmp[j] = '0';
				result = subsets[tmp] + max(cmax - problem_data[index]->term, 0) * problem_data[index]->weight; // OPT(xn,..,xi+1,xi-1,..,x0) + wx*Tx 
				if (print) to_return += "	OPT(" + tmp + ") + w" + to_string(index) + "T" + to_string(index) + " = " + to_string(subsets[tmp]) + " + "
					+ to_string(max(cmax - problem_data[index]->term, 0) * problem_data[index]->weight) + " = " + to_string(result) + "\n";
				rozw = min(rozw, result); // wybranie najmniejszego kosztu dla danego i
				tmp = bitset<number_of_digits>(i).to_string();
			}
		}
		subsets[tmp] = rozw;
	}
	to_return += "Kolejnosc zadan: \n" + findWayForDPA(problem_data, subsets) + "\n";
	to_return += "Najmniejszy uzyskany koszt to " + to_string(rozw) + "\n";

	return to_return;
}

string findWayForDPA(vector <Task*> & problem_data, map <string, unsigned int> & subsets)
{
	vector <unsigned int> road;
	int index = 0, cmax = 0, result = 0, rozw = INT_MAX, best_index = 0;
	string best_subset = "", to_return = "", memory = "";
	string tmp = bitset<number_of_digits>(subsets.size() - 1).to_string();

	while (road.size() != problem_data.size())
	{
		rozw = INT_MAX;
		cmax = 0;
		memory = tmp;
		for (size_t j = 0; j < tmp.length(); j++)
			if (tmp[j] == '1')
				cmax += problem_data[number_of_digits - j - 1]->time;
		for (size_t j = 0; j < tmp.length(); j++)
		{
			if (tmp[j] == '1')
			{
				index = number_of_digits - j - 1;
				tmp[j] = '0';
				result = subsets[tmp] + max(cmax - problem_data[index]->term, 0) * problem_data[index]->weight;
				if (result < rozw)
				{
					best_index = index;
					best_subset = tmp;
				}
				rozw = min(rozw, result);
				tmp = memory;
			}
		}
		road.push_back(best_index);
		tmp = best_subset;
	}

	for (size_t i = 0; i < road.size(); i++)
		to_return += to_string(road[i]) + "  ";

	road.clear();
	return to_return;
}