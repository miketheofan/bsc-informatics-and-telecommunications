#ifndef _INCL_SKIPLIST
#define _INCL_SKIPLIST

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <string.h>
#include <iomanip>

#include "Citizen.hpp"

#define emptyNum -1
#define emptyList "0"
#define emptyCountry NULL

using namespace std;

class SkipList{

public:

	int currentLevel;
	Citizen *citizen;
	SkipList *nextNode;
	SkipList *previousNode;
	SkipList *previousLayer;

public:

	SkipList(int,Citizen*);
	~SkipList();
	void print();
	void printCitizens();

};

class SkipListHeader{

public:

	int maximumLevel;	
	int currentLevel;
	float probability;
	SkipListHeader *nextLayer;
	SkipListHeader *previousLayer;
	SkipList *list;

public:

	SkipListHeader(int,int,float);
	~SkipListHeader();
	void print();
	void printCitizens();
	void insert(Citizen*);
	Citizen* search(int);
	int countTotal(string);
	int* countTotalByAge(string);
	void Delete(int);
	int randomLevelGenerator();
	bool isEmpty();

};

#endif