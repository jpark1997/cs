#ifndef MYSTACK_H
#define MYSTACK_H
#define MAX_SIZE 10000
#include<iostream>

using namespace std;

template <class T>
class mystack{
  private:
        // Declare necessary variables and methods
//  	int maxStack;
	int top1;
	T * stackArr;
  public:
        mystack() {
		stackArr = new T[MAX_SIZE];
		top1 = 0;
	}

        void push(T x) {
	/*	if(top1 == MAX_SIZE) {
			cout << "error" << endl;
			return;
		}
*/
		stackArr[top1++] = x;
	}

        T top() {
	/*	if(top1 == 0) {
	//		cout << "error" << endl;
			return 0;
		} */

//		cout << stackArr[top1-1] << endl;
		return stackArr[top1-1];
	}

        T pop() {
	/*	if(top1 == 0) {
	//		cout << "error" << endl;
			return 0;
		}
	*/	top1--;
//		cout << stackArr[top1] << endl;
		return stackArr[top1];
	}

        bool empty() {
		if(top1 == 0) {
//			cout << "1" << endl;
			return 1;
		} else {
//			cout << "0" << endl;
			return 0;
		}
	}

        int size() {
//		cout << top1 << endl;
		return top1;
	}

};

#endif
