#pragma once
#include <cstdlib>
#include <algorithm>
#include <climits>
#include "Graph.h"

class BF {
	int* best_route;
	int* route;
	int** matrix;
	int count;
	int cost;
	int best_cost;
	

	void calcStartRoute(unsigned long long start, unsigned long long silnia);
	void route_copy();
	int calcCost();

public:
	BF(Graph *graph);
	~BF();

	void BF_solve(unsigned long long, unsigned long long, unsigned long long silnia);
	int* get_route();
	int* get_best_route();
	int get_cost();
	int get_best_cost();
};
