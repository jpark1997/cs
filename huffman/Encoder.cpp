#include <iostream>
#include "HuffTree.h"
#include "MinHeap.h"
#include "TreeNode.h"
#include "Encoder.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <bitset>

using namespace std;

Encoder::Encoder(string file_path, string output_file_path)
{
	this->file_path = file_path;
	this->output_file_path = output_file_path;
//	ofstream of(output_file_path.c_str());
//	MinHeap * mh = new MinHeap();
//	HuffTree * ht = new HuffTree();
}

void Encoder::buildFrequencyTable()
{
	FILE * fp = fopen(file_path.c_str(), "r");
	int c;
	
//	ifstream ff;
//	ff.open(file_path.c_str());
/*
	for(int i = 0; i < 256; i++) {
		frequency_table[i] = 0;
	}
*/	
//	char by;
//	int jj = 0;
	
	c = getc(fp);
	while(c != EOF) {
	//	unsigned char by;
	//	ff >> by;
		body1.push_back(c);
		frequency_table[c]++;
		c = fgetc(fp);
	}

//	ff.clear();
//	ff.seekg(0, ios::beg);
//	fp.close();
}

void Encoder::encode()
{
	buildFrequencyTable();

//	MinHeap * mh = new MinHeap();

	for(int i = 0; i < 256; i++) {
		if(frequency_table[i] != 0) {
			TreeNode * t = new TreeNode(i, frequency_table[i]);
//			cout << "build freq:" << frequency_table[i] << endl;
//			cout << "build char:" << i << endl;
			mh->insert(t);
			uniqp++;
		}
	}
	// building minheap

//	HuffTree * ht = new HuffTree();
	TreeNode * rrr;
	rrr = ht->buildTree(mh);
	//  building huffTree

	ht->generateCodes(rrr);

//	ht->printCodes();
}

void Encoder::writeEncodedFile(string output_file_path)
{
	of.open(output_file_path.c_str());
/*	int uniq = 0;
	for(int i = 0; i < 256; i++) {
		if(frequency_table[i] != 0) {
			uniq++;
//			of << i << endl;
		}
	}
//	string nums = bitset<16>(uniq).to_string();
*/	unsigned short nums;
	nums = (unsigned short)uniqp;

	of.write(reinterpret_cast<const char*>(&nums),sizeof(nums));

	for(int j = 0; j < 256 ; j++) {
		if(frequency_table[j] != 0) {
			unsigned char ch;
			ch = (unsigned char)j;
			//string fr = bitset<32>(frequency_table[j]).to_string();
			unsigned int fr;
			fr = (unsigned int)frequency_table[j];
			of.write(reinterpret_cast<const char*>(&ch), sizeof(ch));
			of.write(reinterpret_cast<const char*>(&fr), sizeof(fr));
		}
	}

//	ff = fopen(output_file_path.c_str(), "w");
//	int c;	
//	ifstream fff;
//	fff.open(file_path.c_str());
//	fff.clear();
//	fff.seekg(0, ios::beg);

	
	unsigned int len = 0;
	for(int k = 0; k < body1.size(); k++) {
		string tt = ht->getCharCode((int)body1[k]);
//		cout << tt << endl;
		stob(tt);
		if(cbit == 8) {
			of.write(reinterpret_cast<const char*>(&buffer), sizeof(buffer));
	//		cout << buffer << endl;
			cbit = 0;
			buffer = 0;
//			if(cbit == 8) {
//				cbit = 0;
//			}
		}
		len = len + tt.length();
	}

	if(len % 8 != 0) {
		int e = len % 8;
		int h = 8-e;
		for(int k = 0; k < h; k++) {
			wbit(0);
	//		buffer <<= 1;
	//		cbit++;
	//		if(cbit >= 8) {
	//			of.write(reinterpret_cast<const char*>(&buffer), sizeof(buffer));
//				cout << "len " << buffer << endl;
	//			cbit = 0;
	//			buffer = 0;
//				if(cbit == 8) {
//					cbit = 0;
//				}
	//		}
		}
	}
	of.close();



}

void Encoder::stob(string st)
{
	for(int i = 0; i < st.length(); i++) {
		if(st[i] == '1') {
			wbit(1);
	//		buffer <<= 1;
	//		buffer |= 0x1;
	//		cbit++;
//			if(cbit == 8) {
//				of.write(reinterpret_cast<const char*>(&buffer), sizeof(buffer));
//				cbit = 0;
//				buffer = 0;
//			}
//		cout << "111111" << endl;
		} else {
			wbit(0);
	//		buffer <<= 1;
	//		cbit++;
//			if(cbit == 8) {
//				of.write(reinterpret_cast<const char*>(&buffer), sizeof(buffer));
//				cbit = 0;
//				buffer = 0;
//			}
//		cout << "000000" << endl;
		}
	}
}

void Encoder::declare()
{
//	ofstream of(output_file_path.c_str());
}

void Encoder::wbit(int bit)
{
	buffer <<= 1;
	if(bit) {
		buffer |= 0x1;
//		cout << "ifbit" << endl;
	} 
	cbit++;
	if(cbit == 8) {
//		cout << "ccccc" << endl;
//		fwrite(&buffer, 1, 1, ff);

//		of.open(output_file_path.c_str());
		of.write(reinterpret_cast<const char*>(&buffer), sizeof(buffer));
		cbit = 0;
		buffer = 0;
	}
}

Encoder::~Encoder()
{
//	delete [] frequency_table;
	delete mh;
	delete ht;
}
