#include <iostream>
#include "HuffTree.h"
#include "MinHeap.h"
#include "TreeNode.h"
#include <string>

using namespace std;

HuffTree::HuffTree()
{
	
}

TreeNode * HuffTree::buildTree(MinHeap *mh)
{
	while(mh->getSize() > 1)
	{
		TreeNode * ni = mh->removeMin();
		TreeNode * nj = mh->removeMin();
		//get two nodes from minheap
		
		TreeNode * pij = new TreeNode(1, ni->getFrequency() + nj->getFrequency());
		// declare a parent node with freq of ni.freq + nj.freq

		pij->join(ni, nj);
		//join ni and nj with parent node pij
//		cout << pij->getFrequency() << endl;
//		cout << "ni, nj:" << ni->getFrequency() << " " << nj->getFrequency() << endl;	
//		cout << "ni, nj ascii:" << ni->getVal() << " " << nj->getVal() << endl;
		mh->insert(pij);
		//insert pij into minhep
//		cout << "insert" << pij->getFrequency() << endl;
//		cout << "pij left:" << pij->getLeft()->getFrequency() << endl;
//		cout << "pij right:" << pij->getRight()->getFrequency() << endl;
		
	//	rootNode = pij;
	}
	rootNode = mh->removeMin();
//	cout << "root freq:" << root1->getFrequency() << endl;
	return rootNode;
}

void HuffTree::generateCodes(TreeNode * root)
{
/*	if(rootNode->getLeft() == NULL && rootNode->getRight() == NULL) {
		codes[rootNode->getVal()].append(codeStr);
	} else if(rootNode->getLeft() != NULL) {
		rootNode = rootNode->getLeft();
		codeStr.append("1");
		generateCodes();
	} else if(rootNode->getRight() != NULL) {
		rootNode = rootNode->getRight();
		codeStr.append("0");
		generateCodes();
	}
*/	
//	if(rootNode->isLeafNode()) {
//		return;
//	}
	string code111 ="";
	visit(root, code111);
}

void HuffTree::visit(TreeNode * root, string codeStr)
{
//	if(root->isLeafNode()) {
//		codes[root->getVal()] = codeStr;
//		cout << "LEAFNODE: "<<root->getVal() << endl;
//	}
	
	//string code222;

	if(root->getLeft() != NULL) {
		string a = codeStr + "1";
		visit(root->getLeft(), a);
	}
	if(root->getRight() != NULL) {
		string a = codeStr + "0";
		visit(root->getRight(), a);
	}

	if(root->isLeafNode()) {
		codes[root->getVal()] = codeStr;
//		cout << "LEAF: " << root->getVal() << " " << codeStr << endl;
	}
}

TreeNode * HuffTree::getRoot()
{
//	return rootNode;
}

string HuffTree::getCharCode(int c)
{
	return codes[c];
}

void HuffTree::printCodes()
{
/*	for(int i = 0; i < 257; i++) {
		if(!codes[i].empty()) {
			cout << codes[i] << endl;
		}
	}*/
}

HuffTree::~HuffTree()
{
//	delete pij
	delete rootNode;
}
