#include "BloomFilter.hpp"

BloomFilter::BloomFilter(int numCells,int numFunctions) : numCells(numCells) , numFunctions(numFunctions){
	
	this->cells = (char*)malloc(sizeof(char)*(this->numCells));
	memset(this->cells,0,numCells);
}

/* The following is a second constructor for class BloomFilter, which takes as an argument (except from all the default arguments that we need to initalize a BloomFilter) 
a bloom array, and copies its' content to this class' bloom array field. */
BloomFilter::BloomFilter(int numCells,int numFunctions,char* cells) : numCells(numCells) , numFunctions(numFunctions){
	
	this->cells = (char*)malloc(sizeof(char)*(this->numCells));

	for(int i=0;i<numCells;i++)
		this->cells[i] = cells[i];
}

int BloomFilter::getNumCells(){
	return this->numCells;
}

int BloomFilter::getNumFunctions(){
	return this->numFunctions;
}

char* BloomFilter::getCells(){
	return this->cells;
}

/* The following function inserts an id in the bloom filter structure. To do that, it uses the hash_i function given and the number of functions given
in the initialization of the structure. */
void BloomFilter::insert(int id){

	string tempString = to_string(id);

	for(int i=0;i<numFunctions;i++){

		unsigned int hashValue = hash_i( (unsigned char*)tempString.c_str() , (unsigned int)i );

		int bitMask = 1u << (7 - hashValue%8);

		this->cells[ (hash_i( (unsigned char*)tempString.c_str() , (unsigned int)i )%this->numCells) / 8 ] |= bitMask;
	
	}
}

/* The following function is used to search for an id in the bloom filter structure. To do that, it again uses the hash_i function and the number of functions
parameter. */
bool BloomFilter::search(int id){

	string tempString = to_string(id);

	for(int i=0;i<numFunctions;i++){

		unsigned int hashValue = hash_i( (unsigned char*)tempString.c_str() , (unsigned int)i );

		int bitMask = 1 << (7 - hashValue%8);

		if((this->cells[ (hash_i( (unsigned char*)tempString.c_str() , (unsigned int)i )%this->numCells) / 8 ] & bitMask) == 0)
			return false;
	}

	return true;
}

BloomFilter::~BloomFilter(){
	free(cells);
}