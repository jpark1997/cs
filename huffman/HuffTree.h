#ifndef HUFFTREE_H
#define HUFFTREE_H

#include "MinHeap.h"
#include "TreeNode.h"
#include <string>

#define MAXCODELENGTH 256
#define BYTESIZE 8

using namespace std;

class HuffTree
{
	private:
		//Add class members and methods	
		TreeNode * rootNode;
//		TreeNode * current;
		string codes[256];
		string codeStr;

	public:
		HuffTree();

		//build a hiffman  tree  give a minheap
		TreeNode * buildTree(MinHeap * mh);

		//generate codes by traversing the huffman tree
		//and store them in an internal data structure (array of strings for example)
		void generateCodes(TreeNode * root);

		void visit(TreeNode * root, string codeStr);

		//returns root of the tree
		TreeNode * getRoot();
		
		void printCodes();

		//returns huffman code from  the ascii code
		string getCharCode(int c);
		~HuffTree();
		
};


#endif
