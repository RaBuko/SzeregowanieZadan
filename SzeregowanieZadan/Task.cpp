#include "Task.h"

std::string Task::toString()
{return std::to_string(time) + " " + std::to_string(weight) + " " + std::to_string(term);}

Task::Task(int i, int c, int w, int t)
	: index(i)
	, time(c)
	, weight(w)
	, term(t)
{}

Task::~Task()
{
}
