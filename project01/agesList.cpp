#include "agesList.hpp"

agesList::agesList(int size): size(size){

	this->ages = (ageNode**)malloc(sizeof(ageNode*)*size);
	
	for(int i=0;i<this->size;i++)
		this->ages[i] = new ageNode;
}

agesList::~agesList(){

	for(int i=0;i<this->size;i++)
		delete this->ages[i];

	free(this->ages);
}

int agesList::hashFunction(int key){
   return key % this->size;
}

/* The following function returns a ageNode*, which for the program represents an age of an entry. */
ageNode* agesList::insert(int age){

	return this->ages[hashFunction(age)]->insert(age);
}

ageNode::ageNode(){

	this->content = -1;
	this->nextNode = NULL;
}

ageNode::~ageNode(){

	if(this->nextNode != NULL)
		delete this->nextNode;
}

int ageNode::getContent(){
	return this->content;
}

/* The following function inserts an age in the hash table, and returns a pointer to the node of the separate chain that this particular age has been inserted at. */
ageNode* ageNode::insert(int age){

	if(this->content == -1){

		this->content = age;
		return this;
	}

	if(this->content == age)
		return this;

	if(this->nextNode == NULL)
		this->nextNode = new ageNode;
	
	return this->nextNode->insert(age);

}