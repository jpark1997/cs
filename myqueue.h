#ifndef MYQUEUE_H
#define MYQUEUE_H
#define MAX_SIZE 10000
#include<iostream>

using namespace std;

template <class T>
class myqueue{
  private:
	  // Declare necessary variables and methods
 	T * qArr;
	int fr;
	int rear;
	int count;

  public:
        myqueue() {
		qArr = new T[MAX_SIZE];
		fr = 0;
		rear = -1;
		count = 0;
	}

    	void enqueue(T x) {
		qArr[++rear] = x;
		count++;
	}

    	T front() {
		return qArr[fr];
	}

        T dequeue() {
		count--;
		return qArr[fr++];
	}

    	bool empty() {
		if(fr > rear) {
			return 1;
		} else {
			return 0;
		}
	}

        int size() {
		return count;
	}

};

#endif
