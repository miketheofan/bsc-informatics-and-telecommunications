#include "stringsList.hpp"

stringsList::stringsList(int size): size(size){

	this->contents = (stringNode**)malloc(sizeof(stringNode*)*size);
	
	for(int i=0;i<this->size;i++)
		this->contents[i] = new stringNode;
}

stringsList::~stringsList(){

	for(int i=0;i<this->size;i++)
		delete this->contents[i];

	free(this->contents);
}

/* The following is the hash function that the hash table uses. It is an implementation of the djb2 hashing algorithm. */
int stringsList::hashFunction(string key){
	
	unsigned long hash = 5381;
	int c;
	int i=0;

	while (c = key[i++])
		hash = ((hash << 5) + hash) + c;

	return hash % this->size;
}

/* The following function returns a nameNode*, which for the program represents a firstName/lastName of an entry. */
stringNode* stringsList::insert(string content){

	return this->contents[hashFunction(content)]->insert(content);
}

stringNode::stringNode(){

	this->content = " ";
	this->nextNode = NULL;
}

stringNode::~stringNode(){

	if(this->nextNode != NULL)
		delete this->nextNode;
}

string stringNode::getContent(){
	return this->content;
}

/* The following function inserts a name in the hash table, and returns a pointer to the node of the separate chain that this particular name has been inserted at. */
stringNode* stringNode::insert(string content){

	if(this->content == " "){

		this->content = content;
		return this;
	}

	if(this->content == content)
		return this;

	if(this->nextNode == NULL)
		this->nextNode = new stringNode;
	
	return this->nextNode->insert(content);

}