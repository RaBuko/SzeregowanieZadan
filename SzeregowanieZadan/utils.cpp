#include "utils.h"


string orderToString(vector <Task*> problemData)
{
	int paddedLength = 3;
	string toReturn = "";
	for (size_t i = 0; i < problemData.size(); i++)
	{
		std::string s = to_string(problemData[i]->index);
		s.insert(s.begin(), paddedLength - s.size(), ' ');
		toReturn += s;
	}
	
	return toReturn;
}


vector<Task*> shuffleOrder(vector <Task*> problemData)
{
	vector <Task*> newOrder = problemData;
	random_shuffle(newOrder.begin(), newOrder.end());
	//for (size_t i = 0; i < newOrder.size(); i++)
	//	newOrder[i]->index = i;
	return newOrder;
}


string BruteForce(vector <Task*> & problem_data, bool print)
{
	vector <unsigned int> t;
	for (size_t i = 0; i < problem_data.size(); i++) t.push_back(i);
	int time = 0, penalty = 0, min_penalty = INT_MAX;
	string best_order = "", to_return = "";

	while (true)
	{

		time = 0;
		penalty = 0;

		for (size_t i = 0; i < t.size(); i++)
		{
			if (print) to_return += to_string(t[i]) + " ";
			time += problem_data[t[i]]->time;
			penalty += max(0, time - problem_data[t[i]]->term) * problem_data[t[i]]->weight;
		}

		if (print) to_return += " | Wynik = " + to_string(penalty);

		if (min_penalty > penalty)
		{
			if (print) to_return += " < BEST";
			best_order.clear();
			min_penalty = penalty;
			for (size_t i = 0; i < t.size(); i++)
				best_order += to_string(t[i]) + " ";
		}

		if (print) to_return += "\n";

		int i = t.size() - 1;
		while (i > 0 && t[i - 1] >= t[i]) i--;

		if (i == 0)
		{
			to_return += "\nNajlepsza kolejnosc : " + best_order + " | Wynik : " + to_string(min_penalty);
			break;
		}

		int j = i;
		while (j < (int)t.size() && t[j] > t[i - 1]) j++;
		j--;
		swap(t[i - 1], t[j]);
		reverse(t.begin() + i, t.begin() + t.size());
	}


	t.clear();
	return to_return;
}


void generateTasks(vector<Task*>& problem_data, unsigned int n)
{
	for (size_t i = 0; i < n; i++)
		problem_data.push_back(new Task(i, rand() % (n * 2) + 1, rand() % (n * 2) + 1, rand() % (n * 2) + 1));
}


void showTasks(vector <Task*> & data)
{
	cout << "\nIlosc zadan : " << data.size() << "\n";
	cout << " N | Czas | Waga | Termin\n";
	for (size_t i = 0; i < data.size(); i++)
		cout << setw(2) << data[i]->index << " | " << setw(4) << data[i]->time << " | " << setw(4) << data[i]->weight << " | " << setw(4) << data[i]->term << "\n";
}


int calculateCost(vector <Task*> arrOfTasks)
{
	int costSum = 0, actualTime = 0;
	for (size_t i = 0; i < arrOfTasks.size(); i++)
	{
		actualTime += arrOfTasks[i]->time;
		costSum += (max(actualTime - arrOfTasks[i]->term, 0) * arrOfTasks[i]->weight);
	}
	return costSum;
}


vector <Task*> loadFromFile(string filename)
{
	vector <Task*> loadedTasks;
	ifstream file;
	string c, w, t;
	int lzadan;
	filename += ".txt";
	file.open(filename);

	if (file.good())
	{
		if (filename[0] == 'w' && filename[1] == 't')
		{
			cout << "Plik " << filename << " znaleziony (typ wt)\n";
			if (filename.substr(2, 3) == "100")
				c = filename.substr(2, 3);
			else c = filename.substr(2, 2);
			lzadan = stoi(c);
			cout << "Liczba zadan: " << lzadan << "\n";


			int i = 0;
			cout << "Przetwarzanie czasow zadan i ich wstepne tworzenie\n";
			while (i < lzadan)
			{
				file >> c;
				Task * task = new Task(i, stoi(c), 0, 0);
				i++;
				loadedTasks.push_back(task);
			}

			i = 0;
			cout << "Przetwarzanie wag zadan\n";
			while (i < lzadan)
			{
				file >> w;
				loadedTasks[i]->weight = stoi(w);
				i++;
			}

			cout << "Przetwarzanie terminow zadan\n";
			i = 0;
			while (i < lzadan)
			{
				file >> t;
				loadedTasks[i]->term = stoi(t);
				i++;
			}
		}
		else
		{
			cout << "Plik " << filename << " znaleziony\n";
			int i = 0;
			while (file >> c)
			{
				file >> w;
				file >> t;
				Task * task = new Task(i, stoi(c), stoi(w), stoi(t));
				i++;
				loadedTasks.push_back(task);
			}
		}
		cout << "Zakonczono\n";
		file.close();
		return loadedTasks;
	}
	else
	{
		cout << "Nie udalo sie wczytac pliku\n";
		cout << "Zakonczono\n";
		file.close();
		return loadedTasks;
	}
	
}


void saveToFile(string filename, vector <Task*> & tasksToSave)
{
	ofstream file;
	file.open(filename, std::ofstream::trunc);
	if (file.good())
	{
		if (tasksToSave.size() == 0)
		{
			cout << "Brak danych do zapisania\n";
			file.close();
			return;
		}
		cout << "Plik istnieje, polaczono\n";
		string data = "";
		for (size_t i = 0; i < tasksToSave.size(); i++)
			data += tasksToSave[i]->toString() + "\n";
		data.pop_back();
		file << data;

	}
	else cout << "Nie udalo sie zapisac do pliku\n";
	file.close();
}


