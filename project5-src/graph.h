#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <utility>
#include <vector>

using namespace std;

class Graph
{
	private:
		int numCities;
		int numRoutes;
		string *cities;
		double weight;

//		int time = 0;
		double** matrix;
//		int* visited;
//		int* discTime;
//		int* low;
//		int* parent;
		vector<pair<string, string>> bv;
		int cityIndex = 0;
		int numBridge = 0;
		string *bc;
		int bi = 0;
	public:
		
		Graph(int, int);

		int min(int, int);

//		int countComp(int n, double** matrix);

//		int getRoot(int* arr, double i);
//		bool sortHelp(pair<string, string> p1, pair<string, string> p2);

		// Create the graph
		void addRoute(string, string, double);	

		void DFS(int i, int visited[], int discTime[], int low [], int parent[]);
		void DFS1(int i, int visited1[], int discTime1[], int low1[], int parent1[]);
		
		//part 1: Find the connected components and the bridges
		void analyseGraph();		
		
		
		// Part 2: Find a ticket using Dijkstra	
		int minD(double dist[], bool set[]);
		void path(int parent[], int j);
		void printHelp(double dist[], int parent[], int dst, int src);
		void findCost(const string &source, const string &destination);         
		
		// Part 3: Try to do a tour of all cities
		void eulerianTour(int root, vector<vector<pair<int, string>>>);  // NOTE : If necessary, you can change the parameters to this function.
		int minKey(int key[], bool mstSet[]);
		void printWeight(int parent[]);
		void mst(string root);
};

#endif
