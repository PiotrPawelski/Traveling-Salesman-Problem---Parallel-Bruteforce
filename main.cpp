#include <iostream>
#include <fstream>
#include <string>
#include "Graph.h"
#include "BF.h"
#include "mpi.h"

using namespace std;

//Calculate the cost of the route
int calcCost(int count, int *route, int **matrix) {
	int tempCost = 0;
	
	for (int i = 0; i < count; i++) 
		tempCost += matrix[route[i]][route[i + 1]];
		
	return tempCost;

}

int main(int argc, char* argv[])
{
	int myid, numprocs, bestCost = 0;
	unsigned long long factorial = 1;

	fstream file;
	Graph *graph;
	BF *bf;

	//Load data
	file.open("input.txt", ios::in);
	
	if (!file.is_open()) {
		cout << "Error opening file!" << endl;
		return 0;
	}

	//Initialize data class
	graph = new Graph(file);
	file.close();

	//Initialize solver class
	bf = new BF(graph);

	int count = graph->getCount();

	//Initialize MPI
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	//Initialize table of routes
	int **routes = new int *[numprocs];

	for (int i = 0; i < numprocs; i++) 
		routes[i] = new int[count + 1];

	for (int i = 1; i < count; i++)
		factorial *= i;

	unsigned long long part = factorial / numprocs;

	//Start solving on all processes
	bf->BF_solve(myid * part, (myid + 1) * part - 1, factorial);

	//Get best route cost on all processes
	int cost = bf->get_best_cost();

	
	if (myid == 0) {  //If main process
		routes[0] = bf->get_best_route();

		//Receive best routes from rest of processes
		for (int i = 1; i < numprocs; i++) 
			MPI_Recv(routes[i], count + 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		
	}
	else //If not main process, send best route to main
		MPI_Send(bf->get_best_route(), count + 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	
	//Calculate best cost from all processes
	MPI_Reduce(&cost, &bestCost, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
	
	int od = factorial % numprocs;

	//If main process
	if (myid == 0) {
		
		//Calculate remainder if exist
		if (od != 0) {
			bf->BF_solve(factorial - od, factorial, factorial);
			if (bf->get_best_cost() < bestCost)
				bestCost = bf->get_best_cost();
		}

		//Calculate final best route
		int *finalRoute = bf->get_best_route();
		
		for (int i = 0; i < numprocs; i++) {
			if (calcCost(count, finalRoute, graph->getMatrix()) > calcCost(count, routes[i], graph->getMatrix())) {
				finalRoute = routes[i];
			}
		}
		
		//Print best route and cost
		for (int i = 0; i <= count; i++)
			cout << finalRoute[i] << endl;

		cout<<bestCost<<endl;
		
		delete bf;
	}

	MPI_Finalize();

	return 0;
}
