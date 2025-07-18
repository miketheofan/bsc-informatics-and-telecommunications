#ifndef _INCL_REQUESTLIST
#define _INCL_REQUESTLIST

#include <string>
#include <iostream>

using namespace std;

#include "functions.hpp"

class RequestList{

private:

	string date;
	string country;
	string virusName;
	bool isAccepted;
	RequestList* next;

public:

	RequestList();
	~RequestList();
	void insert(string,string,string,bool);
	void travelStats(int**,string,string,string);
	void travelStats(int**,string,string,string,string);
	bool isEmpty();

};

#endif