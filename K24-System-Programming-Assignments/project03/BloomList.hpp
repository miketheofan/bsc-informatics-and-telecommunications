#ifndef _BLOOMLIST
#define _BLOOMLIST

#include <string>
#include <bitset>

#include "BloomFilter.hpp"
#include "Citizen.hpp"
#include "SkipList.hpp"
#include "RecordsList.hpp"

bool areSame(Citizen*,Citizen*,int,string);

using namespace std;

class BloomList{

private:

	string virusName;
	int sizeOfBloom;
	int noFunctions;
	int maximumLevel;
	float probability;
	SkipListHeader* notVaccinated;
	SkipListHeader* Vaccinated;
	BloomList* next;

public:
	
	BloomFilter* bf;
	BloomList(string,int,int,int,float);
	~BloomList();
	void insert(Citizen*,string);
	string isVaccinated(int,string);
	string getVirus();
	char* getCells();
	BloomList* getNext();
	void searchVaccinationStatus(RecordsList**,int);
	string getCitizen(int);
	string getAge(int);

};

#endif