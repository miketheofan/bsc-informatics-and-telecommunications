#ifndef _INCL_CITIZEN
#define _INCL_CITIZEN

#include <string>
#include <iostream>

using namespace std;

typedef int citizenID;
typedef string firstName;
typedef string lastName;
typedef string country;
typedef int age;
typedef string virus;
typedef string date;

#define emptyDate "0"

class Citizen{

private:

	citizenID id;
	firstName fn;
	lastName ln;
	country c;
	age a;
	virus vn;
	date d;

public:

	Citizen(citizenID,firstName,lastName,country,age,virus,date);
	~Citizen();
	citizenID getID();
	string getFirstName();
	string getLastName();
	string getCountry();
	int getAge();
	string getVirusName();
	string getDate();
	void print(string);

};

#endif