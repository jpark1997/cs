#ifndef MINHEAP_H
#define MINHEAP_H
#include <string>
#include "TreeNode.h"
#include <vector>

using namespace std;

class MinHeap
{
	private:
		int n;//current number of elements
		vector<TreeNode*> arr;
		//int maxSize;
	public:
		MinHeap();
		TreeNode * removeMin(); //returns root of heap
		void insert(TreeNode * val); //adds element to heap
		int getSize(); //returns size of heap
//		~MinHeap();
};

#endif
