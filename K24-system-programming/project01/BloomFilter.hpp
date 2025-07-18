#ifndef _INCL_BLOOMFILTER
#define _INCL_BLOOMFILTER

#include <iostream>
#include <string>

using namespace std;

unsigned long hash_i(unsigned char*, unsigned int);

class BloomFilter{

private:

	long numCells;
	int numFunctions;
	bool *cells;

public:

	BloomFilter(long,int);
	int getNumCells();
	int getNumFunctions();
	bool* getCells();
	void insert(int);
	bool search(int);
	~BloomFilter();

};

#endif