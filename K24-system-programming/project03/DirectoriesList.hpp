#ifndef _INCL_DIRECTORIESLIST
#define _INCL_DIRECTORIESLIST

#include <string>
#include <iostream>

using namespace std;

#define emptyDirectory " "

class DirectoriesList{

private:

	string country;
	DirectoriesList* next;

public:

	DirectoriesList();
	~DirectoriesList();
	DirectoriesList* getNext();
	string getCountry();
	void insert(string);
	bool countryExists(string);
};

#endif