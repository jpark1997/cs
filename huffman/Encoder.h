#ifndef ENCODER_H
#define ENCODER_H
#include "HuffTree.h"
#include "MinHeap.h"
#include <fstream>
#include <string>

using namespace std;

class Encoder
{
	private:
		unsigned frequency_table[256] = {0};
		// You need to add more class memeber and methods
		string file_path;
		string output_file_path;
		ofstream of;
//		ofstream of1;
		MinHeap * mh = new MinHeap();
		HuffTree * ht = new HuffTree();
		//char body1[10000];
		int cbit = 0;
//		FILE * ff;
		unsigned char buffer = 0;
		int uniqp = 0;
		vector<unsigned char> body1;
	public:
		//vector<unsigned char> body1;
		//test_file_path is the input (decoded) file
		Encoder(string file_path, string output_file_path); 
		
		//Fills up the frequency_table array where frequency_table[i] 
		//will contain the frequency of char with ASCII code i		
		void buildFrequencyTable();

		//Builds the min head and run the encoding algorithm
		void encode();

		//Generates the encoded file and save it as output_file_path
		void writeEncodedFile(string output_file_path);

		void stob(string st);

		void wbit(int bit);

		void declare();
		~Encoder();
};

#endif
