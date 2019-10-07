#pragma once

#include <iostream>
#include <fstream>
#include <climits>

using namespace std;


class Graph {
	
	private:
		//Matrix of costs
		int **matrix;

		//Number of cities
		int count;

	public:
		//Generate random matrix
		Graph(int n);
		
		//Generate matrix from input data
		Graph(std::fstream &data);
		~Graph();

		int **getMatrix();
		int getCount();

};

