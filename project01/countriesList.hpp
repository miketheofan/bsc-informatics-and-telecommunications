#ifndef _INCL_COUNTRIESLIST
#define _INCL_COUNTRIESLIST

#include <string>

#include "SkipList.hpp"

using namespace std;

class countriesList{

private:

	string country;
	countriesList *next;
	int totalCountries;

public:

	countriesList();
	void insert(string);
	int getCount();
	string getCountry();
	countriesList* search(string);
	void populationStatus(string,string,SkipListHeader*,SkipListHeader*);
	void popStatusByAge(string,string,SkipListHeader*,SkipListHeader*);
	~countriesList();

};

#endif