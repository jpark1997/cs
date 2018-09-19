#include<iostream>
#include "myqueue.h"

using namespace std;
/*
template <class T>
myqueue<T>::myqueue() {
	qArr = new T[MAX_SIZE];
	fr = 0;
	rear = -1;
	count = 0;
}

template <class T>
void myqueue<T>::enqueue(T x) {
	qArr[++rear] = x;
	count++;
}

template <class T>
T myqueue<T>::front() {
	if(fr > rear) {
		cout << "error" << endl;
		return NULL;
	}

	cout << qArr[fr] << endl;
	return qArr[fr];
}

template <class T>
T myqueue<T>::dequeue() {
	if(fr > rear) {
		cout << "error" << endl;
		return NULL;
	}
	count--;
	cout << qArr[fr] << endl;
	return qArr[fr++];
}

template <class T>
bool myqueue<T>::empty() {
	if(fr > rear) {
		cout << "1" << endl;
		return true;
	} else {
		cout << "0" << endl;
		return false;
	}
}

template <class T>
int myqueue<T>::size() {
	cout << count << endl;
	return count;
}
*/
int main(){
	string typeString;
	cin >> typeString;

	if(typeString == "int") {
		string command;
		//cin >> command;
		myqueue<int> iq;

		while(cin >> command) {
			if(command == "enqueue") {
				int id;
				cin >> id;
				iq.enqueue(id);
			} else if(command == "front") {
				if(iq.empty() == 1) {
					cout << "error" << endl;
				}
				cout << iq.front() << endl;
			} else if(command == "dequeue") {
				if(iq.empty() == 1) {
					cout << "error" << endl;
				}
				cout << iq.dequeue() << endl;
			} else if(command == "empty") {
				cout << iq.empty() << endl;
			} else if(command == "size") {
				cout << iq.size() << endl;
			}
		}
	} else if(typeString == "double") {
		string command;
		myqueue<double> dq;

		while(cin >> command) {
			if(command == "enqueue") {
				double dd;
				cin >> dd;
				dq.enqueue(dd);
			} else if(command == "front") {
				if(dq.empty() == 1) {
					cout << "error" << endl;
				}
				cout << dq.front() << endl;
			} else if(command == "dequeue") {
				if(dq.empty() == 1) {
					cout << "error" << endl;
				}
				cout << dq.dequeue() << endl;
			} else if(command == "empty") {
				cout << dq.empty() << endl;
			} else if(command == "size") {
				cout << dq.size() << endl;
			}
		}
	} else if(typeString == "char") {
		string command;
		myqueue<char> cq;

		while(cin >> command) {
			if(command == "enqueue") {
				char cd;
				cin >> cd;
				cq.enqueue(cd);
			} else if(command == "front") {
				if(cq.empty() == 1) {
					cout << "error" << endl;
				}
				cout << cq.front() << endl;
			} else if(command == "dequeue") {
				if(cq.empty() == 1) {
					cout << "error" << endl;
				}
				cout << cq.dequeue() << endl;
			} else if(command == "empty") {
				cout << cq.empty() << endl;
			} else if(command == "size") {
				cout << cq.size() << endl;
			}
		}
	} else if(typeString == "string") {
		string command;
		myqueue<string> sq;

		while(cin >> command) {
			if(command == "enqueue") {
				string sd;
				cin >> sd;
				sq.enqueue(sd);
			} else if(command == "front") {
				if(sq.empty() == 1) {
					cout << "error" << endl;
				}
				cout << sq.front() << endl;
			} else if(command == "dequeue") {
				if(sq.empty() == 1) {
					cout << "error" << endl;
				}
				cout << sq.dequeue() << endl;
			} else if(command == "empty") {
				cout << sq.empty() << endl;
			} else if(command == "size") {
				cout << sq.size() << endl;
			}
		}
	}
	cout << "" << endl;

  return 0;
}
