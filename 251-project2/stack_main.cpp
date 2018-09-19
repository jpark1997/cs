#include<iostream>
#include "mystack.h"

using namespace std;
/*
template <class T>
mystack<T>::mystack() {
	stackArr = new T[MAX_SIZE];
	//this->maxStack = maxStack;
	top1 = 0;
}

template <class T>
void mystack<T>::push(T x) {
	if(top1 == MAX_SIZE) {
		cout << "error" << endl;
		return;
	}

	stackArr[top1++] = x;
	//cout << x << endl;
}

template <class T>
T mystack<T>::top() {
	if(top1 == 0) {
		cout << "error" << endl;
		return NULL;
	}

	cout << stackArr[top1-1] << endl;	
	return stackArr[top1-1];
}

template <class T>
T mystack<T>::pop() {
	if(top1 == 0) {
		cout << "error" << endl;
		return NULL;
	}
	top1--;
	cout << stackArr[top1] << endl;
	return stackArr[top1];
}

template <class T>
bool mystack<T>::empty() {
	if(top1 == 0) {
		cout << "1" << endl;
		return true;
	} else {
		cout << "0" << endl;
		return false;
	}
}

template <class T>
int mystack<T>::size() {
	cout << top1 << endl;
	return top1;
}
*/
int main(){
	string ts;
	cin >> ts;
	
//	mystack<int> st;
	if(ts == "int") {
		mystack<int> st;
		
		string command;
		while(cin >> command) {
			if(command == "push") {
				int id;
				cin >> id;
				st.push(id);
			} else if(command == "top") {
				if(st.empty() == 1) {
					cout << "error" << endl;
				} else {
					cout << st.top() << endl;
				}
			} else if(command == "pop") {
				if(st.empty() == 1) {
					cout << "error" << endl;
				} else {
					cout << st.pop() << endl;
				}
			} else if(command == "empty") {
				cout << st.empty() << endl;
			} else if(command == "size") {
				cout << st.size() << endl;
			}
		}
	} else if(ts == "double") {
		mystack<double> st;

		string command;
		while(cin >> command) {
			if(command == "push") {
				double dd;
				cin >> dd;
				st.push(dd);
			} else if(command == "top") {
				if(st.empty() == 1) {
					cout << "error" << endl;
				} else { 
					cout << st.top() << endl;
				}
			} else if(command == "pop") {
				if(st.empty() == 1) {
					cout << "error" << endl;
				} else {
					cout << st.pop() << endl;;
				}
			} else if(command == "empty") {
				cout << st.empty() << endl;
			} else if(command == "size") {
				cout << st.size() << endl;
			}
		}
	} else if(ts == "char") {
		mystack<char> st;

		string command;
		while(cin >> command) {
			if(command == "push") {
				char cd;
				cin >> cd;
				st.push(cd);
			} else if(command == "top") {
				if(st.empty() == 1) {
					cout << "error" << endl;
				} else {
					cout << st.top() << endl;
				}
			} else if(command == "pop") {
//				char popC = st.pop();
				if(st.empty() == 0) {
					cout << "error" << endl;;
				} else { 
					cout << st.pop() << endl;
				}
			} else if(command == "empty") {
				cout << st.empty() << endl;
			} else if(command == "size") {
				cout << st.size() << endl;
			}
		}
	} else if(ts == "string") {
		mystack<string> st;

		string command;
		while(cin >> command) {
			if(command == "push") {
				string sd;
				cin >> sd;
				st.push(sd);
			} else if(command == "top") {
				if(st.empty() == 1) {
					cout << "error" << endl;
				} else {
					cout << st.top() << endl;
				}
			} else if(command == "pop") {
			//	string popS = st.pop();
				if(st.empty() == 1) {
					cout << "error" << endl;
				} else {
					cout << st.pop() << endl;
				}
			} else if(command == "empty") {
				cout << st.empty() << endl;
			} else if(command == "size") {
				cout << st.size() << endl;
			}
		}
	}

	cout << "" << endl;
/*
	string command;
	//cin >> command;
	while(cin >> command) {
		if(command == "push") {
			int id;
			double dd;
			char cd;
			string sd;
			
			if(ts == "int" ) {
				cin >> id;
				st.push(id);
			} else if(ts == "double") {
				cin >> dd;
				st.push(dd);
			} else if(ts == "char") {
				cin >> cd;
				st.push(cd);
			} else if(ts == "string") {
				cin >> sd;
				st.push(sd);
			}

		} else if(command == "top") {
			st.top();
		} else if(command == "pop") {
			st.pop();
		} else if(command == "empty") {
			st.empty();
		} else if(command == "size") {
			st.size();
		}

	}
*/
}
