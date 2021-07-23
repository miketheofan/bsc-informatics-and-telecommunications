#include "BloomFilter.hpp"

BloomFilter::BloomFilter(long numCells,int numFunctions) : numCells(numCells*1000) , numFunctions(numFunctions){

	cells = (bool*)calloc(this->numCells,sizeof(bool));
}

int BloomFilter::getNumCells(){

	return this->numCells;
}

int BloomFilter::getNumFunctions(){

	return this->numFunctions;
}

bool* BloomFilter::getCells(){

	return this->cells;
}

/* The following function inserts an id in the bloom filter structure. To do that, it uses the hash_i function given and the number of functions given
in the initialization of the structure. */
void BloomFilter::insert(int id){

	string tempString = to_string(id);

	for(int i=0;i<numFunctions;i++)
		this->cells[ (hash_i( (unsigned char*)tempString.c_str() , (unsigned int)i )) % this->numCells ]= true;
}

/* The following function is used to search for an id in the bloom filter structure. To do that, it again used the hash_i function and the number of functions
parameter. */
bool BloomFilter::search(int id){

	string tempString = to_string(id);

	for(int i=0;i<numFunctions;i++)
		if( this->cells[ (hash_i( (unsigned char*)tempString.c_str() , (unsigned int)i )) % this->numCells ] == false )
			return false;

	return true;
}

BloomFilter::~BloomFilter(){

	free(cells);
}