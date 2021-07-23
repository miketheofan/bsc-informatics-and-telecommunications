#include "CitizensHash.hpp"

citizensList::citizensList(int size): size(size){

	this->citizens = (HashTable**)malloc(sizeof(HashTable*)*size);
	
	for(int i=0;i<this->size;i++)
		this->citizens[i] = new HashTable;
}

citizensList::~citizensList(){

	for(int i=0;i<this->size;i++)
		delete this->citizens[i];

	free(this->citizens);
}

int citizensList::hashFunction(int key){
   return key % this->size;
}

/* Following function inserts a citizen in the hash table. */
bool citizensList::insert(Citizen* citizen){

	this->citizens[hashFunction(citizen->getID())]->insert(citizen);
	return true;
}

Citizen* HashTable::getCitizen(){
	return this->citizen;
}

/* The following function inserts a citizen in hash table bucket's chain. */
void HashTable::insert(Citizen* citizen){

	if(this->citizen == NULL){

		this->citizen = citizen;
		return;
	}

	if(this->nextNode == NULL)
		this->nextNode = new HashTable;
	
	this->nextNode->insert(citizen);

}

HashTable::HashTable(){

	this->citizen = NULL;
	this->nextNode = NULL;
}

HashTable::~HashTable(){

	if(this->citizen != NULL)
		delete this->citizen;

	if(this->nextNode != NULL)
		delete this->nextNode;
}