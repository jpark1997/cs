#include <iostream>
#include <string>
#include "graph.h"
#include <iterator>
#include <algorithm>
#define INFI 1000000
#include <iomanip>
#include <vector>

using namespace std;

/*
	Graph class for use in Project 5.
	
	This provided code does not include any specific data structure
	for storing a graph, only a list of methods that will be called 
	from main. Based on the project handout and the lecture slides,
	select a method for storing the graph and implement it here.

*/

// Constructor for Graph 
Graph::Graph(int Vertices, int routes)
{
	numCities = Vertices;
	numRoutes = routes;
	cities = new string[numCities];
	matrix = new double*[numCities];
	for(int i = 0; i < numCities; i++) {
		matrix[i] = new double[numCities];
	}

	for(int i = 0; i < numCities; i++) {
		for(int j = 0; j < numCities; j++) {
			matrix[i][j] = 0;
		}
	}
	bc = new string[numCities];
//	visited = new int[numCities];
//	discTime = new int[numCities];
//	low = new int[numCities];
//	parent = new int[numCities];
}

int Graph::min(int a, int b) 
{
	if(a <= b) {
		return a;
	} else {
		return b;
	}
}
/*
int Graph::countComp(int n, double** matrix)
{
	int count = n;

	int* root = new int[n];

	for(int i = 0; i < n; i++) {
		root[i] = i;
	}

	for(int i = 0; i < numCities; i++) {
		double x = matrix[i][0];
		double y = matrix[i][1];

		int xR = getRoot(root, x);
		int yR = getRoot(root, y);

		if(xR != yR) {
			count--;
			root[xR] = yR;
		}
	}

	return count;
}

int Graph::getRoot(int* arr, int i)
{
	while(arr[i] != i) {
		arr[i] = arr[arr[i]];
		i = arr[i];
	}
	return i;
}
*/
void Graph::addRoute(string c1, string c2, double w)
{/*
	int c1i = cityIndex;
	cities[cityIndex++] = c1;
	int c2i = cityIndex;
	cities[cityIndex++] = c2;

	matrix[c1i][c2i] = w;
*/
	int found1 = 0;
	int found2 = 0;

	int c1i;
	int c2i;

	for(int i = 0; i < numCities; i++) {
		if(cities[i] == c1) {
			found1 = 1;
			c1i = i;
		}
		if(cities[i] == c2) {
			found2 = 1;
			c2i = i;
		}
	}

	if(found1 == 0) {
		c1i = cityIndex;
		cities[cityIndex++] = c1;
	}
	if(found2 == 0) {
		c2i = cityIndex;
		cities[cityIndex++] = c2;
	}

	matrix[c1i][c2i] = w;
	matrix[c2i][c1i] = w;

}

void Graph::DFS1(int i, int visited1[], int discTime1[], int low1[], int parent1[])
{
	static int time = 0;
	visited1[i] = 1;

	discTime1[i] = time;
	low1[i] = time++;

	for(int j = 0; j < numCities; j++) {
	//	if(matrix[i][j] != 0) {
			if(visited1[j] != 1  && matrix[i][j] != 0) {
				parent1[j] = i;
				DFS1(j, visited1, discTime1, low1, parent1);
				low1[i] = min(low1[i], low1[j]);
//				cout << "??" << endl;
//				if(low1[j] > discTime1[i]) {
//				}
			} else if(j != parent1[i]) {
				low1[i] = min(low1[i], discTime1[j]);
			}
	//	}
	}
}

void Graph::DFS(int i, int visited[], int discTime[], int low[], int parent[])
{
	static int time = 0;
	visited[i] = 1;
	
	discTime[i] = time;
	low[i] = time++;
//	cout << low[i] << " ";

	for(int j = 0; j < numCities; j++) {
//		cout << "dfs for" << endl;
//		cout << visited[j] << endl;
//		cout << matrix[i][j] << endl;
		if(matrix[i][j] != 0) {
//			if(cities[i] == "BBD") {
//				cout << "??" << i << endl;
//			}
			if(visited[j] == 0) {
				parent[j] = i;
				//low[i] = discTime[i];
				DFS(j, visited, discTime, low, parent);
//				cout << "dfs if1111" << endl;
//				cout << low[i] << " " << low[j];
//				cout << "dfs1" << endl;
				low[i] = min(low[i], low[j]);
//				if(cities[i] == "AAC" && cities[j] == "BBD") {
//					cout << "!!!!" << endl;
//				}
				if(low[j] > discTime[i]) {
				//bridge
//					cout << cities[i] << " " << cities[j] << " ";
//					cout << "bridgeeeeeeeeee" << endl;
//					cout << i << " " << cities[j] << " ";
					numBridge++;
					//bc[bi++] = cities[i];
					//bc[bi++] = cities[j];
					if(cities[i].compare(cities[j]) < 0) {
						bv.push_back(make_pair(cities[i], cities[j]));
					} else if(cities[i].compare(cities[j]) > 0) {
						bv.push_back(make_pair(cities[j], cities[i]));
					}
				}
			} else if(j != parent[i]) {
				low[i] = min(low[i], discTime[j]);
			}
		}
	}
}

bool sortHelp(pair<string, string> p1, pair<string, string> p2)
{
	if(get<0>(p1).compare(get<0>(p2)) < 0) {
		return true;
	} else if(get<0>(p1).compare(get<0>(p2)) == 0) {
		if(get<1>(p1).compare(get<1>(p2)) < 0) {
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

void Graph::analyseGraph()
{
	int* visited = new int[numCities];
	int* discTime = new int[numCities];
	int* low = new int[numCities];
	int* parent = new int[numCities];

	int comp = 0;

	for(int i = 0; i < numCities; i++) {
//	for(int j = 0; j < numCities; j++) {
//		if(visited[i] != 1) {
			DFS(i, visited, discTime, low, parent);
//			cout << "dfs" << endl;
//			comp++;
//		} 
//			cout << "dfs" << endl;
	}
//	}

	int* visited1 = new int[numCities];
	int* discTime1 = new int[numCities];
	int* low1 = new int[numCities];
	int* parent1 = new int[numCities];

	for(int i = 0; i < numCities; i++) {
		if(visited1[i] == 0) {
			DFS1(i, visited1, discTime1, low1, parent1);
			comp++;
		}
	}

	

//	DFS(0);
	
	sort(bv.begin(), bv.end(), sortHelp);		
	cout << comp << endl;
	cout << numBridge << endl;
	for(int k = 0; k < numBridge; k++) {
		cout << bv[k].first << " " << bv[k].second << endl;
	}

}

int Graph::minD(double dist[], bool set[])
{
	int min = INFI;
	int min_ind;

	for(int v = 0; v < numCities; v++)
		if(set[v] == false && dist[v] <= min)
			min = dist[v], min_ind = v;

	return min_ind;
}

void Graph::path(int parent[], int j)
{
	if(parent[j] == -1)
		return;

	path(parent, parent[j]);

	cout << " " << cities[j];
}

void Graph::printHelp(double dist[], int parent[], int dst, int src)
{
	cout << cities[src];
	path(parent, dst);
	cout << fixed;
	cout << setprecision(2);
	cout << " " << dist[dst] << endl;
	//printf(" %f\n", dist[dst]);
}

void Graph::findCost(const string &source, const string &destination)
{
	int src;
	int dst;

	double dist[numCities];

	bool set[numCities];

	int parent[numCities];

	int* visited = new int[numCities];
	int* discTime = new int[numCities];
	int* low = new int[numCities];
	int* parent1 = new int[numCities];
	int comp = 0;

	for(int i = 0; i < numCities; i++) {
		if(cities[i] == source) {
			src = i;
		}
		
		if(cities[i] == destination) {
			dst = i;
		}

		parent[i] = -1;
		dist[i] = INFI;
		set[i] = false;
	
		if(visited[i] == 0) {
			DFS1(i, visited, discTime, low, parent1);
			comp++;
		}
	}

	if(comp > 1) {
		cout << "not possible" <<endl;
		exit(1);
	}

	dist[src] = 0;

	for(int i = 0; i < numCities - 1; i++) {
		int u = minD(dist, set);

		set[u] = true;

		for(int v = 0; v < numCities; v++) {
			if(!set[v] && matrix[u][v] && dist[u] + matrix[u][v] < dist[v]) {
				parent[v] = u;
				dist[v] = dist[u] + matrix[u][v];
			}
		}
	}

	printHelp(dist, parent, dst, src);
}

int Graph::minKey(int key[], bool mstSet[])
{
	int min = INFI;
	int minInd;

	for(int v = 0; v < numCities; v++) {
		if(mstSet[v] == false && key[v] < min) {
			min = key[v];
			minInd = v;
		}
	}

	return minInd;
}

void Graph::printWeight(int parent[])
{
	double sum = 0;

	for(int i = 1; i < numCities; i++) {
//		cout << parent[i] << " " << i << endl;
//		cout << cities[parent[i]] << " " << cities[i] << endl;
		sum += matrix[i][parent[i]];
	}
	//cout << "a" << endl;
	cout << sum << endl;
}

void Graph::eulerianTour(int root, vector<vector<pair<int, string>>> euler)
{
	cout << cities[root] << endl;
	int size = euler.at(root).size();
	//cout << size;
	for(int i = 0; i < size; i++) {
	//	cout << "for";
		eulerianTour(get<0>(euler.at(root).at(i)), euler);
	}
}

bool sortChild(pair<int, string> p1, pair<int, string> p2)
{
	if(get<1>(p1).compare(get<1>(p2)) < 0) {
		return true;
	} else {
		return false;
	}
}

void Graph::mst(string root)
{
	int parent[numCities];
	int key[numCities];
	bool mstSet[numCities];
	vector<vector<pair<int, string>>> euler;

	int* visited = new int[numCities];
	int* discTime = new int[numCities];
	int* low = new int[numCities];
	int* parent1 = new int [numCities];
	int comp = 0;

	int rootIndex;
	for(int i = 0; i < numCities; i++) {
		key[i] = INFI;
		mstSet[i] = false;
		if(cities[i] == root) {
			rootIndex = i;
		}
		if(visited[i] == 0) {
			DFS1(i, visited, discTime, low, parent1);
			comp++;
		}
	}
	if(comp > 1) {
		cout << "not possible" << endl;
		exit(1);
	}

	key[rootIndex] = 0;
	parent[rootIndex] = -1;

	for(int k = 0; k < numCities - 1; k++) {
		int u = minKey(key, mstSet);

		mstSet[u] = true;
//cout << "for";
		for(int v = 0; v < numCities; v++) {
			//cout << "2";
			if(matrix[u][v] && mstSet[v] == false && matrix[u][v] < key[v]) {
				//cout << "if";
				parent[v] = u;
				key[v] = matrix[u][v];
			}
		}
	}

	for(int i = 0; i < numCities; i++) {
		vector<pair<int, string>> child;
		for(int j = 0; j < numCities; j++) {
			if(parent[j] == i) {
				child.push_back(make_pair(j, cities[j]));
			}
		}
		sort(child.begin(), child.end(), sortChild);
		euler.push_back(child);
	}
/*	
	int rootIndex;
	for(int i = 0; i < numCities; i++) {
		if(cities[i] == root) {
			rootIndex = i;
		}
	}*/
	eulerianTour(rootIndex, euler);
//	printWeight(parent);
}
