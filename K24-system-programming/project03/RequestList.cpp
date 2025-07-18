#include "RequestList.hpp"

RequestList::RequestList(){

	this->next = NULL;
	this->virusName = " ";
}

RequestList::~RequestList(){
	
	if(this->next != NULL)
		delete this->next;
}

/* The following function inserts an entry in requestList. */
void RequestList::insert(string date,string country,string virusName,bool isAccepted){

	if(this->virusName == " "){

		this->date = date;
		this->country = country;
		this->virusName = virusName;
		this->isAccepted = isAccepted;
		
		return;
	}

	if(this->next == NULL)
		this->next = new RequestList;

	this->next->insert(date,country,virusName,isAccepted);
}

/* The following function fills results array with the correct results (according to dates given) of this 
RequestList's. This function is used only if country is not given. */
void RequestList::travelStats(int** results,string virusName,string date1,string date2){

	cout << "Entered for " << virusName << " " << date1 << " " << date2 << endl;

	if(isLater(this->date,date1) && !isLater(this->date,date2) && this->virusName == virusName){

		(*results)[0]++;

		if(this->isAccepted)
			(*results)[1]++;
		else
			(*results)[2]++;
	}

	if(this->next != NULL) this->next->travelStats(results,virusName,date,date2);
}

/* The following function fills results array with the correct results (according to dates given) of this 
RequestList's. This function is used only if country is given. */
void RequestList::travelStats(int** results,string virusName,string date1,string date2,string country){

	if(isLater(this->date,date1) && !isLater(this->date,date2) && this->virusName == virusName && this->country == country){

		(*results)[0]++;

		if(this->isAccepted)
			(*results)[1]++;
		else
			(*results)[2]++;

		if(this->next != NULL) this->next->travelStats(results,virusName,date,date2);
	}
}

bool RequestList::isEmpty(){
	return this->virusName == " " ? true : false;
}