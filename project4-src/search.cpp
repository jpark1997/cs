#include <iostream>
#include <vector>
#include "RBTree.h"
#include "Node.h"
#include <algorithm>
#include <string>
#include <iterator>
#include <fstream>
using namespace std;

// Input stream operator overload for node elements
istream& operator>>(istream& in, vector<pair<string, int>>& e);

bool comp(pair<string, int> p1, pair<string, int> p2) {
	if(get<0>(p1).compare(get<0>(p2)) < 0) {
		return true;
	} else {
		return false;
	}
}

bool compint(pair<string, int> p1, pair<string, int> p2) {
//	return (get<1>(p1) > get<1>(p2));
	if(get<1>(p1) == get<1>(p2)) {
		return comp(p1, p2);
	} else {
		return (get<1>(p1) > get<1>(p2));
	}
}

vector<pair<string, int>> getIntersection(vector<pair<string, int>> v1, vector<pair<string, int>> v2) {
	vector<pair<string, int>> result;

	for(int i = 0; i < v1.size(); i++) {
		for(int j = 0; j < v2.size(); j++) {
			if(get<0>(v1.at(i)).compare(get<0>(v2.at(j))) == 0) {
				int freq = get<1>(v1.at(i)) + get<1>(v2.at(j));
				pair<string, int> pp = make_pair(get<0>(v1.at(i)), freq);
				result.push_back(pp);
			}
		}
	}

	return result;
}

int main(int argc, char** argv) {
	// Implement your search code here
	string input = argv[1];
	ifstream f(input);
	RBTree<string, vector<pair<string, int>>> rbt;
	f >> rbt;
	vector<vector<pair<string, int>>> list;

	for(int i = 2; i < argc; i++) {
		string word = argv[i];
		transform(word.begin(), word.end(), word.begin(), ::tolower);

		Node<string, vector<pair<string, int>>>* node = rbt.search(word);
		
		if(node != NULL) {
			vector<pair<string, int>> tempE = node->getElement();
			list.push_back(tempE);
		} else {
			list.push_back({});
		}
	}

	int c = 2;
	vector<pair<string, int>> b1;

	sort(list.at(0).begin(), list.at(0).end(), comp);

	if(list.size() > 1) {	
		sort(list.at(1).begin(), list.at(1).end(), comp);
//	vector<pair<string, int>> b1;
		//set_intersection(list.at(0).begin(), list.at(0).end(), list.at(1).begin(), list.at(1).end(), back_inserter(b1));
		b1 = getIntersection(list.at(0), list.at(1));
	} else {
		b1 = list.at(0);
	}

	while(c < list.size()) {
		sort(list.at(c).begin(), list.at(c).end(), comp);
		//set_intersection(b1.begin(), b1.end(), list.at(c).begin(), list.at(c).end(), back_inserter(b1));
		b1 = getIntersection(b1, list.at(c));
		c++;
	}
	sort(b1.begin(), b1.end(), comp);
	sort(b1.begin(), b1.end(), compint);
//	sort(b1.begin(), b1.end(), comp);
	if(b1.empty()) {
		cout << "Not found" << endl;
	} else {
		for(int k = 0; k < b1.size(); k++) {
			cout << get<0>(b1.at(k)) << " " << get<1>(b1.at(k)) << endl;
		}
	}

	return 0;
}

// This function overloads the input stream operator for Node
// elements. It has been provided to you and does not need to be
// altered.
istream& operator>>(istream& in, vector<pair<string, int>>& e) {
	// Clear any existing contents
	e.clear();

	// Read the number of elements
	size_t n;
	in >> n;
	for (int i = 0; i < n; i++) {
		// Read each member of each pair the vector stores
		string u;
		in >> u;
		int f;
		in >> f;
		// Add to vector
		e.push_back(make_pair(u, f));
	}

	return in;
}
