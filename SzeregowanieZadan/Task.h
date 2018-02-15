#pragma once
#include <iostream>
#include <string>

class Task
{
public:
	int index, time, weight, term;

	std::string toString();
	Task(int index, int time, int weight, int term);
	~Task();
};

