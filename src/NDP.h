#ifndef NDP_hpp
#define NDP_hpp

#include <stdio.h>
#include <vector>

struct NDP
{
	double x;
	double y;

	bool closed;
	bool connected;

	int id;

	std::vector<double> continuousSolution;
	std::vector<int> integerSolution;
};

#endif 
