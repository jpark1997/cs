#include <iostream>
#include "Decoder.h"
#include "TreeNode.h"
#include "MinHeap.h"
#include "HuffTree.h"
#include <fstream>
#include <vector>

using namespace std;

Decoder::Decoder(string huff_file_path, string file_path)
{
	this->huff_file_path = huff_file_path;
	this->file_path = file_path;
}

void Decoder::buildFrequencyTableFromFile()
{
//	ifstream f;
//	unsigned short num;
//	f.open(huff_file_path.c_str(), std::ios::in | std::ios::binary);
	f.read(reinterpret_cast<char *>(&num), sizeof(num));
	//read number of chars from encoded

	for(int i = 0; i < num; i++) {
		unsigned char ch;
		unsigned int freq;
		f.read(reinterpret_cast<char *>(&ch), sizeof(ch));
		f.read(reinterpret_cast<char *>(&freq), sizeof(freq));
		frequency_table[ch] = freq;
//		cout << ch << " " << freq << endl;
	}
	//input to freq table
}

void Decoder::decode()
{
	f.open(huff_file_path.c_str(), std::ios::in | std::ios::binary);
	buildFrequencyTableFromFile();
	
	for(int i = 0; i < 256; i++) {
		if(frequency_table[i] != 0) {
			TreeNode * t = new TreeNode(i, frequency_table[i]);
			min->insert(t);
		}
	}
	
//	TreeNode * root;
	root = huff->buildTree(min);

	writeUncompressedFile(file_path);
	
	f.close();
}

void Decoder::writeUncompressedFile(string file_path)
{
	int count = 0;
	ofstream out;
	out.open(file_path.c_str());
	
	while(!f.eof()) {
		char byte;
		f.read(reinterpret_cast<char *>(&byte), sizeof(byte));
		for(int i = 7; i >= 0; i--) {
			int bit = (byte >> i) & 1;
			bits.push_back(bit);
//			cout << bit << endl;
		}
	}
	//read bits and store them in int vector
	
	int index = 0;
	curr = root;
	int freq = root->getFrequency();
	while(count < freq) {
		
		if(bits[index] == 1) {
			curr = curr->getLeft();
		} else if(bits[index] == 0) {
			curr = curr->getRight();
		}
		index++;
		if(curr->isLeafNode()) {
			decoded += curr->getVal();
		//	cout << decoded << endl;
			curr = root;
			count++;
	//		cout << count << " " << decoded << endl;
		}
	}
	//traverse hufftree according to bits

	out << decoded;
	out.close();
}
/*
char Decoder::visit(TreeNode * root)
{
	if(root->getLeft() != NULL) {
		return 257;
	}
	
	if(root->getRight() != NULL) {
		return 258;
	}

	if(root->isLeafNode()) {
		decoded += root->getVal();
	}
}
*/

Decoder::~Decoder()
{
//	delete []frequency_table;
	delete min;
	delete huff;
}
