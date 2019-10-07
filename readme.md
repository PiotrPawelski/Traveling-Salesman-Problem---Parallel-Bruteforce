#Traveling Salesman Problem - Parallel Bruteforce
====================

*Build:*
mpiCC BF.cpp Graph.cpp main.cpp -o tsp

*Run:*
mpirun -np number\_of\_processes -host list\_of\_hosts ./tsp

For more information about building and running programs under MPI check [this](https://mpitutorial.com/) website.

Program loads data from file placed in the same folder named input.txt.

Example data files are included in this repository under names "tsp_number.txt".