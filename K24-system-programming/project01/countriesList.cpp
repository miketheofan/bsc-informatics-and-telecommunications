#include "countriesList.hpp"

countriesList::countriesList(){

	this->next = NULL;
}

string countriesList::getCountry(){
	return this->country;
}

countriesList* countriesList::search(string country){

	if(this->country.empty()){ 

		this->insert(country);
		return this;
	}

	if(this->country == country) return this;

	if(this->next != NULL) return this->next->search(country);
	else{

		this->next = new countriesList;
		this->next->insert(country);
		return this->next;
	}
}

/*The following function is used to insert a country in Countries' list. In that list, we also
insert new nodes after the first node, in order to save some time upon insertion. */
void countriesList::insert(string country){

	if(this->country.empty()){

		this->country = country;
		return;
	}

	if(this->country == country) return;

	if(this->next == NULL){

		countriesList *newList = new countriesList;

		newList->next = this->next;
		this->next = newList;
	}

	return this->next->insert(country);
}

/* The following function is used, in populationStatus function, and it basically traverses
every country in our record and for each one prints the results needed, using the List::populationStatus function. 
It is basically used for when user doesn't give a country as argument, so results must be printed for all countries. */
void countriesList::populationStatus(string date1,string date2,SkipListHeader *list,SkipListHeader* nonList){

	int totalNotVaccinated = nonList->countTotal(this->getCountry());
	
	list->populationStatus(totalNotVaccinated,date1,date2,this);

	if(this->next != NULL) this->next->populationStatus(date1,date2,list,nonList);
}

/*Same exact thing for popStatusByAge function. */
void countriesList::popStatusByAge(string date1,string date2,SkipListHeader *list,SkipListHeader* nonList){

	int* totalNotVaccinated = nonList->countTotalByAge(this->getCountry());
	
	list->popStatusByAge(totalNotVaccinated,date1,date2,this);

	if(this->next != NULL) this->next->popStatusByAge(date1,date2,list,nonList);
}

/* The following function returns the total amount of countries we have in our record. */
int countriesList::getCount(){

	if(this->next != NULL)
		return 1+this->next->getCount();
	else
		return 1;
}

countriesList::~countriesList(){

	if(this->next != NULL) delete this->next;
}