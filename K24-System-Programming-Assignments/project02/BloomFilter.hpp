#ifndef _INCL_BLOOMFILTER
#define _INCL_BLOOMFILTER

#include <iostream>
#include <string>
#include <string.h>

using namespace std;

unsigned long hash_i(unsigned char*, unsigned int);

class BloomFilter{

private:

	int numCells;
	int numFunctions;
	char *cells;

public:

	BloomFilter(int,int);
	BloomFilter(int,int,char*);
	int getNumCells();
	int getNumFunctions();
	char* getCells();
	void insert(int);
	bool search(int);
	~BloomFilter();

};

#endif