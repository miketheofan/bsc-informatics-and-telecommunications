#include "virusesList.hpp"

virusesList::virusesList(string name , int maximumLevel , float probability , long numOfCells , int numOfFunctions) 
: virus(name) , maximumLevel(maximumLevel) , probability(probability) , numOfCells(numOfCells) , numOfFunctions(numOfFunctions){

	this->nextVirus = NULL;
	this->vaccinated_persons = new SkipListHeader(maximumLevel,0,probability);
	this->not_vaccinated_persons = new SkipListHeader(maximumLevel,0,probability);
	this->bf = new BloomFilter(numOfCells,numOfFunctions);
}

string virusesList::getName(){
	return this->virus;
}

virusesList* virusesList::getNextVirus(){
	return this->nextVirus;
}

void virusesList::print(){

	cout << this->virus << ":" << endl << endl << "VACCINATED:" << endl;
	this->vaccinated_persons->print();
	cout << endl << "NOT VACCINATED: " << endl;
	this->not_vaccinated_persons->print();
	cout << endl;
	if( this->nextVirus != NULL ) this->nextVirus->print();
}

/* The following function inserts a virus in the List structure. In other words, it creates a new node
that is dedicated to the new virus. */
virusesList* virusesList::insertVirus(string name){

	if( this->virus == emptyList ){
	
		this->virus = name;
		return this;
	}

	if( this->virus == name )		
		return this; /* If a node already exists for given virus, the function returns. */

	if( this->nextVirus == NULL ) this->nextVirus = new virusesList(name,this->maximumLevel,this->probability,this->numOfCells,this->numOfFunctions);

	return this->nextVirus->insertVirus(name);
}

/* The following function inserts a citizen to the List. */
void virusesList::insertCitizen(Citizen *citizen,string info){

	if( citizen->getVirusName() == this->virus ) /* Firstly, it searches for the right virus, according to Citizen::virusName variable */

		if(info == "YES"){ /* If it is a record saying that citizen is vaccinated to the specific virus */

			/* Then we insert his data to the correct structures(bloom filter and vaccinated persons skip list). */
			this->bf->insert(citizen->getID());
			this->vaccinated_persons->insert(citizen);
		}else /* Otherwise we insert his data to the not vaccinated skip list. */
			this->not_vaccinated_persons->insert(citizen);
	else
		
		if( this->nextVirus != NULL ) this->nextVirus->insertCitizen(citizen,info);
		else{

			/* If there isn't a node for the specific virus, create one */
			this->nextVirus = new virusesList(citizen->getVirusName(),this->maximumLevel,this->probability,this->numOfCells,this->numOfFunctions);
			this->nextVirus->insertCitizen(citizen,info);
		}
}

/* The following function deletes a citizen from the not vaccinated persons skip list. This function is used mostly when a new citizen record is entered
with information about a person gettting vaccinated for a virus. In that case, we have to delete his entry from the not vaccinated list and insert him to
the vaccinated persons skip list of the virus. */
void virusesList::deleteCitizen(Citizen* citizen){

	if( citizen->getVirusName() == this->virus ){

		this->not_vaccinated_persons->Delete(citizen->getID());
	}else
		if( this->nextVirus != NULL ) this->nextVirus->deleteCitizen(citizen);
}

/* The following function returns whether a citizen is vaccinated for a virus given or not, using the bloom filter structure for the virus given. */
bool virusesList::vaccineStatusBloom(citizenID id,string virus){

	if(this->virus == virus){

		if(this->bf->search(id) == true) return true;
		else return false;
	}

	if(this->nextVirus != NULL)
		return this->nextVirus->vaccineStatusBloom(id,virus);

	return false;
}

/* The following function returns whether a citizen is vaccinated or not for a certain virus given. using the vaccinated persons skip list structure. */
bool virusesList::vaccineStatus(citizenID id,string virus){

	Citizen* tempCitizen;

	if(this->virus == virus){

		if( (tempCitizen = this->vaccinated_persons->search(id)) != NULL){

			cout << "VACCINATED ON: " << tempCitizen->getDate() << endl;
			return true;
		}else return false;
	}

	if(this->nextVirus != NULL) return this->nextVirus->vaccineStatus(id,virus);

	return false;
}

/* The following function returns whether a citizen exists in the not vaccinated persons skip list of a virus given or not. */
bool virusesList::findOnNotVaccinated(citizenID id,string virus){

	Citizen* tempCitizen;

	if(this->virus == virus){

		if( (tempCitizen = this->not_vaccinated_persons->search(id)) != NULL )
			return true;
		else return false;
	}

	if(this->nextVirus != NULL) return this->nextVirus->findOnNotVaccinated(id,virus);

	return false;
}

/* The following function checks if there is a citizen in the record that has the same data as the arguments given. */
bool virusesList::existsWithDiffData(citizenID id,string fn,string ln,countriesList* c,int a, string v){

	Citizen* tempCitizen;

	if(this->virus == v){

		if( (tempCitizen = this->not_vaccinated_persons->search(id)) != NULL ){
			
			if(tempCitizen->getID() == id && tempCitizen->getFirstName() == fn && tempCitizen->getLastName() == ln 
			&& tempCitizen->getCountry() == c->getCountry() && tempCitizen->getAge() == a && tempCitizen->getVirusName() == v)
				return false;
			else return true;
		}else return false;
	}

	if(this->nextVirus != NULL) return this->nextVirus->existsWithDiffData(id,fn,ln,c,a,v);
	else return false;
}

/* This function is a version of the vaccineStatus function that is basically used by other functions in order to determine
if a citizen is vaccinated or not. The point of this function is to now print out the messages that the original function does. */
Citizen* virusesList::vaccineStatus(int noMessages,citizenID id,string virus){

	Citizen* tempCitizen;

	if(this->virus == virus){

		if( (tempCitizen = this->vaccinated_persons->search(id)) != NULL)
			return tempCitizen;
		else return NULL;
	}

	if(this->nextVirus != NULL) return this->nextVirus->vaccineStatus(0,id,virus);

	return NULL;
}

/* The following function finds if citizen given has an entry on any skip list for any virus. */
void virusesList::vaccineStatus(citizenID id){

	Citizen *tempVaccinated , *tempNotVaccinated;

	if( (tempVaccinated = this->vaccinated_persons->search(id)) != NULL )
		cout << this->virus << " YES " << tempVaccinated->getDate() << endl;

	if( (tempNotVaccinated = this->not_vaccinated_persons->search(id)) != NULL )
		cout << this->virus << " NO" << endl;

	if(this->nextVirus != NULL) this->nextVirus->vaccineStatus(id);
}

/* The following function lists all entries in the not vaccinated persons skip list of a given virus. */
void virusesList::list_nonVaccinated_Persons(string virus){

	if(this->virus != virus){

		if(this->nextVirus != NULL)
			this->nextVirus->list_nonVaccinated_Persons(virus);
		
		return;
	}

	if(this->not_vaccinated_persons->isEmpty() == false)
		this->not_vaccinated_persons->printCitizens();
}

/* The following function inserts a citizen record on the vaccinated persons skip list of given virus. */
void virusesList::vaccinateNow(stringsList* dates,citizensList* list,citizenID id,stringNode* fn,stringNode* ln,countriesList* c,ageNode* a,string v){

	Citizen *tempCitizen;
	
	time_t t = time(0);
    tm* now = localtime(&t);

    string date;

    /* Firstly, it checks if citizen is already vaccinated for the virus given. */
	if( (tempCitizen = this->vaccineStatus(0,id,v)) != NULL)
		cout << "ERROR: CITIZEN " << tempCitizen->getID() << " ALREADY VACCINATED ON " << tempCitizen->getDate() << endl; 
	/* Then it checks if citizen's id already exists in not vaccinated list with different data. */
	else if( this->existsWithDiffData(id,fn->getContent(),ln->getContent(),c,a->getContent(),v) == true )
		cout << "ERROR: CITIZEN EXISTS WITH DIFFERENT DATA" << endl;
	else{

		/* Following cases exist in order to fill date parts. For example: 3-3-2010 must be 03-03-2010, for the shake of my 
		implementation. */
		string day = to_string(now->tm_mday);
		if(day.length() == 1)
			day.insert(0,1,'0');
		
		string month = to_string(now->tm_mon+1);
		if(month.length() == 1)
			month.insert(0,1,'0');

		string year = to_string(now->tm_year+1900);

    	date.append(day + "-" + month + "-" + year);
    	
		Citizen *newCitizen = new Citizen(id,fn,ln,c,a,this->insertVirus(v),dates->insert(date));

		this->insertCitizen(newCitizen,"YES");

		/* Lastly, it checks if citizen is recorded as not vaccinated for the current virus before, and if yes it deletes him from 
		the not vaccinated persons skip list. */
		if(this->findOnNotVaccinated(newCitizen->getID(),newCitizen->getVirusName()) == true)
			this->deleteCitizen(newCitizen);

	}
}

/* The following function insert a citizen in the List. */
void virusesList::insertCitizenRecord(citizensList* list,citizenID id,stringNode* fn,stringNode* ln,countriesList* c,ageNode* a,string v,string i,stringNode* d){

	Citizen *tempCitizen;

	/* First it checks whether citizen already exists in both skip lists. */
	if( (tempCitizen = this->vaccineStatus(0,id,v)) != NULL ) 	
		cout << "ERROR: CITIZEN " << tempCitizen->getID() << " ALREADY VACCINATED ON " << tempCitizen->getDate() << endl; 
	/* then it checks if entry has "NO" as info, and if citizen already exists on virus' not vaccinated skip list. */
	else if( i == "NO" && this->findOnNotVaccinated(id,v) == true )
		cout << "ERROR: CITIZEN " << id << " ALREADY NOT VACCINATED ON VIRUS" << endl;
	/* and finally it checks if citizen's id already exists in not vaccinated list with different data. */
	else if( i == "YES" && this->existsWithDiffData(id,fn->getContent(),ln->getContent(),c,a->getContent(),v) == true )
		cout << "ERROR: CITIZEN EXISTS WITH DIFFERENT DATA" << endl;
	else{

		Citizen *newCitizen = new Citizen(id,fn,ln,c,a,this->insertVirus(v),d);
		
		if(list->insert(newCitizen,-1) == false)
			return;

		/* If not it inserts him in the List. */
		this->insertCitizen(newCitizen,i);

		/* Lastly, it checks if citizen is recorded as not vaccinated for the current virus before, and if yes it deletes him from 
		the not vaccinated persons skip list. */
		if( i == "YES" &&  this->findOnNotVaccinated(newCitizen->getID(),newCitizen->getVirusName()) == true)
			this->deleteCitizen(newCitizen);

	}
}

/* The following function prints the percentage of vaccinated persons that are vaccinated from country given inside the span
of dates given. */
void virusesList::populationStatus(countriesList *countries,string virus,string date1,string date2,countriesList* country){

	/* If data is for node's virus */
	if(this->virus == virus){

		if(country != emptyCountry) /* If country given is not "0"(emptyCountry) then call the corresponding vaccinated
		persons function. */{

			int totalNotVaccinated = this->not_vaccinated_persons->countTotal(country->getCountry());
			this->vaccinated_persons->populationStatus(totalNotVaccinated,date1,date2,country);
		
		}else{/* Otherwise call the same function but in the countriesList structure, in order to print results
			for every country, as no country was given. */

			countries->populationStatus(date1,date2,this->vaccinated_persons,this->not_vaccinated_persons);
		}

	}else{

		if(this->nextVirus != NULL) this->nextVirus->populationStatus(countries,virus,date1,date2,country);
		else cout << "Virus does not exist." << endl;
	}
}

/* The following function prints the percentage of vaccinated persons that are vaccinated from country given inside the span
of dates given, ordering them in certain age spans, as mentioned by the instructors of the project. The process is the same
as in the above function, but with different function prototypes. */
void virusesList::popStatusByAge(countriesList* countries,string virus,string date1,string date2,countriesList* country){

	if(this->virus == virus){

		if(country != emptyCountry){

			int* totalNotVaccinated = this->not_vaccinated_persons->countTotalByAge(country->getCountry());
			this->vaccinated_persons->popStatusByAge(totalNotVaccinated,date1,date2,country);

		}
		else{

			countries->popStatusByAge(date1,date2,this->vaccinated_persons,this->not_vaccinated_persons);
		}

	}else{

		if(this->nextVirus != NULL) this->nextVirus->popStatusByAge(countries,virus,date1,date2,country);
		else cout << "Virus does not exist." << endl;
	}
}

virusesList::~virusesList(){

	delete this->bf;
	delete this->vaccinated_persons;
	delete this->not_vaccinated_persons;

	if(this->nextVirus != NULL)
		delete this->nextVirus;
}