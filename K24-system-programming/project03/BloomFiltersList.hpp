#ifndef _INCL_BLOOMFILTERSLIST
#define _INCL_BLOOMFILTERSLIST

#include <string>

using namespace std;

#include "BloomFilter.hpp"

class BloomFiltersList{

public:

	int sizeOfBloom;
	int noFunctions;
	string virusName;
	BloomFilter *bf;
	BloomFiltersList *next;

	BloomFiltersList(int,int);
	void insert(string,char*);
	bool isVaccinated(int,string);
	~BloomFiltersList();	
	void print();

};

#endif