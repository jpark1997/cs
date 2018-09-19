#ifndef DECODER_H
#define DECODER_H
#include <fstream>
#include <string>
#include "HuffTree.h"
#include "MinHeap.h"
#include "TreeNode.h"
#include <vector>

using namespace std;



class Decoder
{
	private:
		unsigned frequency_table[256] = {0};
		unsigned short num;
		vector<int> bits;
		string huff_file_path;
		string file_path;
		string decoded;
		ifstream f;
		MinHeap * min = new MinHeap();
		HuffTree * huff = new HuffTree();
		TreeNode * root;
		TreeNode * curr;
		// You need to add more class memeber and methods

	public:
		//huff_file_path is the input (encoded) file that we 
		//want to decode
		Decoder(string huff_file_path, string file_path);

		//Fills up the frequency_table array where frequency_table[i] 
		//will contain the frequency of char with ASCII code i			
		//This method will read the header of the encoded file to 
		//extract the chars and their frequency
		void buildFrequencyTableFromFile();

		//Creates a min-heap and builds the Huffman tree
		void decode();

		//Writes the uncompressed file and save it as file_path
		void writeUncompressedFile(string file_path);
		
	//	void visit(TreeNode * root);
		~Decoder();

};

#endif
