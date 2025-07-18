#ifndef _INCL_CITIZENS
#define _INCL_CITIZENS

#include <string>
#include <iostream>

#include "stringsList.hpp"
#include "agesList.hpp"

using namespace std;

class countriesList;
class virusesList;

typedef int citizenID;

#define emptyDate "0"
#define emptyDateNode NULL

class Citizen{

private:

	citizenID id;
	stringNode* fn;
	stringNode* ln;
	countriesList* c;
	ageNode* a;
	virusesList* vn;
	stringNode* d;

public:

	Citizen(citizenID,stringNode*,stringNode*,countriesList*,ageNode*,virusesList*,stringNode*);
	~Citizen();
	citizenID getID();
	string getFirstName();
	string getLastName();
	string getCountry();
	int getAge();
	string getVirusName();
	string getDate();
	void print();

};

#endif