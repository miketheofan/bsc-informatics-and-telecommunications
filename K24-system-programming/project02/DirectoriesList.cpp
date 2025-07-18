#include "DirectoriesList.hpp"

DirectoriesList::DirectoriesList(){

	this->country = emptyDirectory;
	this->next = NULL;
}

DirectoriesList::~DirectoriesList(){
	if(this->next != NULL) delete this->next;
}

DirectoriesList* DirectoriesList::getNext(){
	return this->next;
}

string DirectoriesList::getCountry(){
	return this->country;
}

/* The following function inserts a country in DirectoriesList. */
void DirectoriesList::insert(string country){

	if(this->country == emptyDirectory){

		this->country = country;
		return;
	}

	if(this->next == NULL)
		this->next = new DirectoriesList();

	this->next->insert(country);
}

/* The following function checks if country exists in DirectoriesList. */
bool DirectoriesList::countryExists(string country){

	if(this->country == country)
		return true;

	return this->next != NULL ? this->next->countryExists(country) : false;
}