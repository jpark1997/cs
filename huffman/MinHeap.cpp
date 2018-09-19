#include <iostream>
#include <climits>
#include "TreeNode.h"
#include "MinHeap.h"
#include <vector>
#include <assert.h>
#define ileft(p) (2*(p) +1)
#define iright(p) (2*(p) +2)
#define iparent(ch) (((ch)-1)/2)

using namespace std;

//implement the methods in MinHeap.h

MinHeap::MinHeap()
{
	n = 0;
}

void MinHeap::insert(TreeNode * val)
{
	arr.push_back(val);
	//n++;
//	arr[n] = val;
	n++;

	int child = arr.size()-1;
	int parent = iparent(child);

	while(child > 0) {
		if(arr[child]->getFrequency() >= arr[parent]->getFrequency()) {
			break;
		}

		TreeNode* temp = arr[child];
		arr[child] = arr[parent];
		arr[parent] = temp;
		
		child = parent;
		parent = iparent(child);
	}
}

int MinHeap::getSize()
{
	return n;
}

TreeNode * MinHeap::removeMin()
{
	if(n == 0) {
		return NULL;
	}
//	arr.pop_back();

	TreeNode* minKey = arr.front();
	arr.pop_back();

	arr[0] = arr[n-1];
	n--;

	if(n == 0) {
		return minKey;
	}

//	arr[0] = arr[n];
//	arr.pop_back();

	int parent = 0;
	int left = ileft(parent);
	int right = iright(parent);

	while(left < n) {
		int minChild = left;

		if(right < n && arr[right]->getFrequency() < arr[left]->getFrequency()) {
			minChild = right;
		}

		if(arr[parent]->getFrequency() <= arr[minChild]->getFrequency()) {
			break;
		}

		if(arr[parent]->getFrequency() > arr[left]->getFrequency() && arr[parent]->getFrequency() > arr[right]->getFrequency()) {
			if(arr[left]->getFrequency() > arr[right]->getFrequency()) {
				minChild = right;
			}
			if(arr[right]->getFrequency() > arr[left]->getFrequency()) {
				minChild = left;
			}
			if(arr[right]->getFrequency() == arr[left]->getFrequency()) {
				minChild = left;
			}
		}

		TreeNode* temp = arr[minChild];
		arr[minChild] = arr[parent];
		arr[parent] = temp;
		
		parent = minChild;
		left = ileft(parent);
		right = iright(parent);
	}
	return minKey;
}
/*
MinHeap::~MinHeap()
{

}
*/
