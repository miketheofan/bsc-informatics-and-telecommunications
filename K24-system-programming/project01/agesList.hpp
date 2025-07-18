#ifndef _INCL_AGES
#define _INCL_AGES

#include <string>
#include <iostream>

class ageNode{

private:

	int content;
	ageNode *nextNode;

public:

	ageNode();
	~ageNode();
	ageNode* insert(int);
	int getContent();

};

class agesList{

private:

	ageNode **ages;
	int size;

public:
	
	agesList(int);
	~agesList();
	ageNode* insert(int);
	int hashFunction(int);

};

#endif