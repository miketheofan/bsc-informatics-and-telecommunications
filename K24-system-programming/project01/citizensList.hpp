#ifndef _INCL_CITIZENSLIST
#define _INCL_CITIZENSLIST

#include "Citizens.hpp"

class HashTable{

private:

	Citizen* citizen;
	HashTable *nextNode;

public:

	HashTable();
	~HashTable();
	void insert(Citizen*);
	Citizen* search(int);
	Citizen* getCitizen();

};

class citizensList{

private:

	HashTable **citizens;
	int size;

public:
	
	citizensList(int);
	~citizensList();
	bool insert(Citizen*,int);
	bool foundSame(Citizen*,int);
	int hashFunction(int);

};

#endif