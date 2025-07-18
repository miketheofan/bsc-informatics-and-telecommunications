#include "BloomFiltersList.hpp"

/* The following function initializes an "empty" BloomList node. */
BloomFiltersList::BloomFiltersList(int sizeOfBloom,int noFunctions) : sizeOfBloom(sizeOfBloom) , noFunctions(noFunctions){
	this->bf = NULL;
	this->next = NULL;
}

/* The following function inserts a bloomArray in this current BloomList. */
void BloomFiltersList::insert(string virusName,char* bloomArray){

	if(this->bf == NULL){

		this->bf = new BloomFilter(sizeOfBloom,noFunctions,bloomArray);
		this->virusName = virusName;
		return;
	}

	if(this->next == NULL)
		this->next = new BloomFiltersList(this->sizeOfBloom,this->noFunctions);

	this->next->insert(virusName,bloomArray);
}

BloomFiltersList::~BloomFiltersList(){
	delete this->bf;
	if(this->next != NULL)
		delete this->next;
}

/* The following function checks whether given id and virusName is vaccinated in BloomList's bloomArray. */
bool BloomFiltersList::isVaccinated(int id,string virusName){

	if(this->virusName == virusName)
		return this->bf->search(id);
	else
		if(this->next != NULL)
			return this->next->isVaccinated(id,virusName);

	return false;
}

/* The following function prints bloomCells of this and all next nodes of BloomList. */
void BloomFiltersList::print(){

	cout << "Virus: " << this->virusName << " bloom: " << this->bf->getCells() << endl;
	if(this->next != NULL)
		this->next->print();
}