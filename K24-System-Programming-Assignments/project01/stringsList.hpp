#ifndef _INCL_NAMES
#define _INCL_NAMES

#include <string>
#include <iostream>

using namespace std;

class stringNode{

private:

	string content;
	stringNode *nextNode;

public:

	stringNode();
	~stringNode();
	stringNode* insert(string);
	string getContent();

};

class stringsList{

private:

	stringNode **contents;
	int size;

public:
	
	stringsList(int);
	~stringsList();
	stringNode* insert(string);
	int hashFunction(string);

};

#endif