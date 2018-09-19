#include <iostream>
#include <string>
#include "MinHeap.h"
#include "Encoder.h"
#include "Decoder.h"
#include <cstring>

using namespace std;

void printHelp(){
	cout << "Invalid arguments." << endl;
	cout << "Type \"huffman [mode] [arguments]\" where mode and its arguments are one of the following:" << endl;
	cout << "-e [src] [dest]: encode the source file and place the result into the specified destination" << endl;
	cout << "-d [src] [dest]: decode the source file and place the result into the specified destination" << endl;
	exit(1);
}


int main (int argc, char** argv){
//	printHelp();
	if(argc != 4) {
		printHelp();
	}

	if(strcmp(argv[1], "-e") == 0) {
		Encoder* e = new Encoder(argv[2], argv[3]);
		e->encode();
		e->writeEncodedFile(argv[3]);
		delete e;
		return 0;
	} else if(strcmp(argv[1], "-d") == 0) {
		Decoder* d = new Decoder(argv[2], argv[3]);
		d->decode();
		delete d;
		return 0;
	}
}
