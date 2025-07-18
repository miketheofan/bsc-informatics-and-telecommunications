#ifndef _INCL_CITIZENSHASH
#define _INCL_CITIZENSHASH

#include "Citizen.hpp"

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
	bool insert(Citizen*);
	int hashFunction(int);

};

#endif