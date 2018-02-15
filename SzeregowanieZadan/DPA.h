#pragma once

#include "utils.h"

// Wykorzystuje algorytm programowania dynamicznego do rozwiazania problemu szeregowania zadan
string DPA(vector <Task*> & problem_data, map <string, unsigned int> & subsets, bool print);

// Znajduje droge wedlug uzyskanych rozwiazan dla kolejnych podzbiorow problemu
// Note: droga jest odwrocona
string findWayForDPA(vector <Task*> & problem_data, map <string, unsigned int> & subsets);

