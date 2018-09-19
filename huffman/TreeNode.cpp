#include <iostream>
#include "TreeNode.h"

#include <string>


TreeNode::TreeNode(unsigned char val, unsigned int frequency)
{
	this->left = NULL;
	this->right = NULL;
	this->val = val;
	this->frequency = frequency;
}

bool TreeNode::isLeafNode() 
{
	if(left == NULL && right == NULL) {
		return true;
	} else {
		return false;
	}
}

unsigned TreeNode::getFrequency()
{
	return frequency;
}
unsigned TreeNode::getVal()
{
	return val;
}

TreeNode * TreeNode::getLeft()
{
	return left;
}

TreeNode * TreeNode::getRight()
{
	return right;
}

void TreeNode::join(TreeNode * left, TreeNode * right)
{
	this->left = left;
	this->right = right;
}

TreeNode::~TreeNode()
{
	delete left;
	delete right;
}
