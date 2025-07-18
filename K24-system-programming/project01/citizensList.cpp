#include "citizensList.hpp"

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

/* Following function inserts a citizen in the hash table. Integer given as argument is used for the program to specify
from where the function is being called, in order to print out the correct message in the foundSame() function. 
If i=0, then function is called from fill() and if i=-1 then function is called from insertCitizenRecord(). */
bool citizensList::insert(Citizen* citizen, int i){

	if(this->citizens[hashFunction(citizen->getID())]->getCitizen() != NULL && this->foundSame(citizen,i) == true){

		delete citizen;
		return false;
	}

	this->citizens[hashFunction(citizen->getID())]->insert(citizen);
	return true;
}

/* The following function is used to compare two citizens. For my implementation, I count two citizens
as same if they have the same ID and refer to a record for the same virus. That is because we can have multiple
records about a citizen, but for different virus each time. 
Function also checks if there is an entry entered with same id but different name,age,etc. In that case, entry
also counts as same and program print error.*/
bool citizensList::foundSame(Citizen* citizen,int i){

	Citizen* tempCitizen;

	if( ( tempCitizen = citizens[hashFunction(citizen->getID())]->search(citizen->getID()) ) == NULL)
		return false;

	if( (tempCitizen->getID() == citizen->getID() && tempCitizen->getVirusName() == citizen->getVirusName())
		|| (tempCitizen->getID() == citizen->getID() && (tempCitizen->getFirstName() != citizen->getFirstName() 
			|| tempCitizen->getLastName() != citizen->getLastName() || tempCitizen->getAge() != citizen->getAge() 
			|| tempCitizen->getCountry() != citizen->getCountry()) )){

		if(i == 0){

			cout << "ERROR IN RECORD ";
			citizen->print();
		}else
			cout <<"ERROR: ID ALREADY EXISTS IN ENTRY WITH DIFFERENT DATA" << endl;

		return true;
	}

	return false;
}

/* The following function searches for a citizen's id in hash table bucket's chain. */
Citizen* HashTable::search(int id){

	if(this->citizen->getID() == id)
		return this->citizen;

	if(this->nextNode != NULL)
		return this->nextNode->search(id);

	return NULL;
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