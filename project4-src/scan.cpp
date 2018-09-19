#include <iostream>
#include <vector>
#include <fstream>
#include "RBTree.h"
#include "SimpleHTMLParser.h"
#include <utility>
#include <queue>
#include "Node.h"
#include <sstream>
using namespace std;

// Output stream operator overload for node elements
ostream& operator<<(ostream& out, const vector<pair<string, int>>& e);
/*
queue<string> split(string words) {
	queue<string> result;
	string w;
	int i = 0;
	while(words[i]) {
		if(words[i] == ' ') {
			if(w.empty()) {
				continue;
			} else {
				result.push(w);
				w.clear();
				i++;
			}
		} else {
			w += words[i];
			i++;
		}
	}

	return result;
}
*/
int main(int argc, char** argv) {
	// Implement your scan code here
	SimpleHTMLParser shp;
	RBTree<string, vector<pair<string, int>>> rbt;
	
	string inputfilename = argv[1];
//	cout << inputfilename << endl;
	ifstream urf(inputfilename);
	string nums;
	getline(urf, nums);
	int num = atoi(nums.c_str());

	for(int i = 0; i < num; i++) {
//		cout << i << endl;
		string url;
		//string words;
		getline(urf, url);
		shp.parse(url);
		stringstream words(shp.getText());
		
		queue<string> wv;

		string tempword;

		while(words >> tempword) {
			wv.push(tempword);
		}
		
		while(!wv.empty()) {
			string cw = wv.front();
			wv.pop();

//			if(cw == " ") {
//				continue;
//			}

			Node<string, vector<pair<string, int>>>* node = rbt.search(cw);

			if(node == NULL) {
				//c.ii
				vector<pair<string, int>> list; //c.ii.1
				pair<string, int> p;
				p = make_pair(url, 1);
				
				list.push_back(p);
				//c.ii.2
				rbt.insert(cw, list);
			} else {
				//c.iii
				vector<pair<string, int>>& list1 = node->getElement();
				//1111111
				int vi = 0;
				int found = 0;
				while(vi < list1.size()) {
					pair<string, int> p1;
					p1 = list1.at(vi);
					if(url == get<0>(p1)) {
						found = 1;
						int vc = get<1>(p1);
						vc++;
						pair<string, int> p2 = make_pair(url, vc);
						list1.at(vi) = p2;
					//	list1.second++;
						break;
					}
					vi++;
				}
				//////22222222&&&&&33333333

				if(found == 0) {
					pair<string, int> p3 = make_pair(url, 1);
					list1.push_back(p3);
				}
				////////4444444444
			}
		}
	}
	

	ofstream out(argv[2]);
	out << rbt;
	out << "" << endl;
	return 0;
}
/*
queue<string> split(string words) {
	queue<string> result;
	string w;
	int i = 0;
	while(words[i]) {
		if(words[i] == ' ') {
			result.push(w);
			w.clear();
			i++
		} else {
			w += words[i];
			i++;
		}
	}

	return result;
}
*/
// This function overloads the output stream operator for Node
// elements. It has been provided to you and does not need to be
// altered.
ostream& operator<<(ostream& out, const vector<pair<string, int>>& e) {
	// Write the number of elements in the vector
	out << e.size() << endl;
	for (auto& uf : e)
		// Write each member of each pair the vector stores
		out << uf.first << " " << uf.second << endl;
	return out;
}
