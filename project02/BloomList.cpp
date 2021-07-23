#include "BloomList.hpp"

BloomList::BloomList(string virusName,int sizeOfBloom,int noFunctions,int maximumLevel,float probability) 
: virusName(virusName) , sizeOfBloom(sizeOfBloom) , noFunctions(noFunctions) , maximumLevel(maximumLevel) , probability(probability) {

	this->next = NULL;
	this->bf = new BloomFilter(this->sizeOfBloom,this->noFunctions);
	this->notVaccinated = new SkipListHeader(this->maximumLevel,0,this->probability);
	this->Vaccinated = new SkipListHeader(this->maximumLevel,0,this->probability);
}

BloomList::~BloomList(){
	delete this->bf;
}

/* The following function inserts a citizen in the BloomList. info is used in order for the list to know whether to insert the citizen
in the vaccinated skip list or in the notVaccinated skip list. */
void BloomList::insert(Citizen* citizen,string info){

	Citizen* tempCitizen;

	/* First we check if citizen already exists using the methods we used for checking in project 1. */
	if(info == "YES")
		tempCitizen = this->Vaccinated->search(citizen->getID());
	else
		tempCitizen = this->notVaccinated->search(citizen->getID());

	if(tempCitizen != NULL && areSame(citizen,tempCitizen,0,info)){

		delete citizen;
		return;
	}

	if(this->virusName == "0")
		this->virusName = citizen->getVirusName();

	if(this->virusName != citizen->getVirusName()){

		if(this->next == NULL)
			this->next = new BloomList(citizen->getVirusName(),this->sizeOfBloom,this->noFunctions,this->maximumLevel,this->probability);
			
		this->next->insert(citizen,info);
		return;
	}

	if(info == "YES"){

		this->bf->insert(citizen->getID());
		this->Vaccinated->insert(citizen);
	}
	else
		this->notVaccinated->insert(citizen);
}

/* The following function checks whether a citizen with id given is vaccinated for virus given using out skip list. */
string BloomList::isVaccinated(int id,string virus){

	if(this->virusName != virus){

		if(this->next == NULL){

			cout << "Virus " << virus << " does not exist." << endl;
			return " ";

		}else
			return this->next->isVaccinated(id,virus);
	}

	return this->Vaccinated->search(id) == NULL ? " " : this->Vaccinated->search(id)->getDate();
}

char* BloomList::getCells(){
	return this->bf->getCells();
}

string BloomList::getVirus(){
	return this->virusName;
}

BloomList* BloomList::getNext(){
	return this->next;
}

/* The following function fills a RecordsList given as a parameter with the data of this list. */
void BloomList::searchVaccinationStatus(RecordsList** rl,int id){

	Citizen* tmpCitizen;
	if( (tmpCitizen = this->Vaccinated->search(id)) != NULL )
		(*rl)->insert(this->virusName + " VACCINATED ON " + tmpCitizen->getDate());
	else
		(*rl)->insert(this->virusName + " NOT YET VACCINATED");

	if(this->next != NULL)
		this->next->searchVaccinationStatus(rl,id);
}

/* The following function returns data from a citizen with id given as a parameter. */
string BloomList::getCitizen(int id){

	Citizen* tmpCitizen;
	
	if( (tmpCitizen = this->Vaccinated->search(id)) != NULL )
		return to_string(tmpCitizen->getID()) + " " + tmpCitizen->getFirstName() + " " + tmpCitizen->getLastName() + " " + tmpCitizen->getCountry();

	if( (tmpCitizen = this->notVaccinated->search(id)) != NULL )
		return to_string(tmpCitizen->getID()) + " " + tmpCitizen->getFirstName() + " " + tmpCitizen->getLastName() + " " + tmpCitizen->getCountry();

	if(this->next != NULL)
		return this->next->getCitizen(id);

	return " ";
}

/* The following function returns the age of a citizen with id given as a paramter. */
string BloomList::getAge(int id){

	Citizen* tmpCitizen;
	
	if( (tmpCitizen = this->Vaccinated->search(id)) != NULL )
		return "AGE " + to_string(tmpCitizen->getAge());

	if( (tmpCitizen = this->notVaccinated->search(id)) != NULL )
		return "AGE " + to_string(tmpCitizen->getAge());

	if(this->next != NULL)
		return this->next->getAge(id);

	return " ";
}