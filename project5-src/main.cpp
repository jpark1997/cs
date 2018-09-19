#include <iostream>
#include <string>
#include "graph.h"
 
using namespace std;
 

 // Part 1 : Find the connected components and the bridge
void part1()
{
	int numC;
	int numE;
	cin >> numC;
	cin >> numE;

	Graph* g = new Graph(numC, numE);

	string a;
	string b;
	double w;

	for(int i = 0; i < numE; i++) {
		cin >> a;
		cin >> b;
		cin >> w;
		g->addRoute(a, b, w);
	}

	g->analyseGraph();
}

 // Part 2 :  Find a ticket using Dijkstra	
void part2()
{
	int numC;
	int numE;
	cin >> numC;
	cin >> numE;

	Graph* g = new Graph(numC, numE);

	string a;
	string b;
	double w;

	for(int i = 0; i < numE; i++) {
		cin >> a;
		cin >> b;
		cin >> w;
		g->addRoute(a, b, w);
	}

	string src;
	string dest;
	cin >> src;

	while(src != "END") {
		cin >> dest;
		g->findCost(src, dest);
		cin >> src;
	}
}

 // Part 3 :Try to do a tour of all cities 
void part3()
{
	int numC;
	int numE;
	cin >> numC;
	cin >> numE;

	Graph* g = new Graph(numC, numE);

	string a;
	string b;
	double w;

	for(int i = 0; i < numE; i++) {
		cin >> a;
		cin >> b;
		cin >> w;
		g->addRoute(a, b ,w);
	}
	
	string root;
	cin >> root;
	g->mst(root);
}
 
 int main()
 {
	 double cmd;
	 cin >> cmd;

	 if(cmd == 1) part1();
	 else if(cmd == 2) part2();
	 else if(cmd == 3) part3();

	 return 0;
 }
