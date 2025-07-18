#ifndef _INCL_LIST
#define _INCL_LIST

#include <string>

#include "SkipList.hpp"
#include "BloomFilter.hpp"
#include "Citizens.hpp"
#include "citizensList.hpp"
#include "countriesList.hpp"
#include "stringsList.hpp"
#include "agesList.hpp"

#include <ctime>
#include <iostream>

using namespace std;

class virusesList{

private:

	string virus;
	BloomFilter *bf;
	SkipListHeader *vaccinated_persons;
	SkipListHeader *not_vaccinated_persons;
	virusesList *nextVirus;
	int maximumLevel;
	float probability;
	long numOfCells;
	int numOfFunctions;

public:

	virusesList(string,int,float,long,int);
	~virusesList();
	string getName();
	virusesList* getNextVirus();
	virusesList* insertVirus(string);
	void insertCitizen(Citizen*,string);
	void deleteCitizen(Citizen*);
	void print();
	bool findOnNotVaccinated(citizenID,string);
	bool vaccineStatusBloom(citizenID,string);
	bool vaccineStatus(citizenID,string);
	Citizen* vaccineStatus(int,citizenID,string);
	void vaccineStatus(citizenID);
	void list_nonVaccinated_Persons(string);
	void vaccinateNow(stringsList*,citizensList*,citizenID,stringNode*,stringNode*,countriesList*,ageNode*,string);
	void insertCitizenRecord(citizensList*,citizenID,stringNode*,stringNode*,countriesList*,ageNode*,string,string,stringNode* d = emptyDateNode);
	void populationStatus(countriesList*,string,string,string,countriesList* c = emptyCountry);
	void popStatusByAge(countriesList*,string,string,string,countriesList* c = emptyCountry);
	bool existsWithDiffData(citizenID,string,string,countriesList*,int,string);

};

#endif