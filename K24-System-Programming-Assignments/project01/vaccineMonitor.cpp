#include "vaccineMonitor.hpp"

int main(int argc , char** argv){

	srand(time(NULL));

	long bloomSize;
	int maximumLevel,maximumNoAges;
	string citizenRecordsFile;

	/* Check if command line arguments are as many as they should be according to the given format: 
		./vaccineMonitor -c citizenRecordsFile -b bloomSize. */
	if(argc != expectedArgs){

		cout << "Wrong input.Exiting program..." << endl;
		return 0;
	}

	/* Get the data we need from argv. */
	for(int i=0;i<argc;i++)

		if(!strcmp(argv[i],"-c"))
			citizenRecordsFile = argv[++i];
		else if(!strcmp(argv[i],"-b"))
			bloomSize = stol(argv[++i]);

	maximumLevel = log(numberOfEntries); /* Maximum level is a parameter that will be used in the initialization of the skip list,
							and it sets the maximum height that a node can extend itself in the skip list. For this certain program
							maximumLevel is set as log(total population on earth).*/

	maximumNoAges = 120; /* Maximum number of ages is a parameter that will be used for the initialization of the agesList hash table.
						It is initialized as 120, as any age that the bash script can produce is inside the range: [1,120]. */

	/* First, we create a data structure that holds all countries from where entries that are inserted, come from. */
	countriesList countries;

	/* Then, we create a data structure that holds all the citizens that will be inserted. */
	citizensList citizens(maximumLevel);

	/* Then we create a data structure that holds all the first names of the citizens that might be inserted. */
	stringsList firstNames(maximumLevel);

	/* Then we create a data structure that holds all the last names of the citizens that might be inserted. */
	stringsList lastNames(maximumLevel);

	/* Then we create a data structure that holds all the ages of the citizens that might be inserted. */
	agesList ages(maximumNoAges);

	/* Then we create a data structure that holds all the dates of the citizens that might be inserted. */
	stringsList dates(maximumLevel);

	/* Finally we create a data structure, which holds for every virus there is, a skiplist for vaccinated persons, a skiplist for not vaccinated
	persons and a bloom filter. */
	virusesList viruses(emptyList,maximumLevel,probability,bloomSize,numOfFunctions);

	/* And then we fill the data structure with the citizenRecordsFile. We also give all the parameters needed for the initialization of the structures. */
	fill(citizenRecordsFile,&viruses,&citizens,&countries,&firstNames,&lastNames,&ages,&dates);

	string input ,function;

	/* Here we read input from the user, and depending on what he gives, we call the corresponding List's function. 
	In many parts of the following code, we check if some strings are empty. That's because we have to check cases that arguments
	in brackets are not given, when user gives us a command. Apart from that, we check every possible case of error input, for every
	function that might be called. In many parts also we use lowestDate and highestDate variables. Those two variables are used in functions that can be called
	without date spans, in which case we show results for a date span bigger than the biggest one that can be produced in the bash script.*/
	while(1){

		cout << endl << "Give me your action: ";

		getline(cin,input);
		istringstream inputStream(input);
		inputStream >> function;

		if(function == "/vaccineStatusBloom"){
			
			string citizenID , virusName;
			inputStream >> citizenID >> virusName;

			if(citizenID.empty() || virusName.empty()){

				cout << "ERROR.MUST GIVE TWO ARGUMENTS." << endl;
				continue;
			}else if(isWord(citizenID) == false){

				cout << "ERROR.FIRST ARGUMENT MUST BE A DIGIT" << endl;
				continue;
			}else if(isDate(virusName) == true){

				cout << "ERROR.SECOND ARGUMENT MUST NOT BE A DATE" << endl;
				continue;
			}else if(isWord(virusName) == true){

				cout << "ERROR.SECOND ARGUMENT MUST BE A WORD" << endl;
				continue;
			}

			if( viruses.vaccineStatusBloom(stoi(citizenID),virusName) == true)
				cout << "MAYBE" << endl;
			else
				cout << "NOT VACCINATED" << endl;			
				
		}else if(function == "/vaccineStatus"){

			string citizenID , virusName;
			inputStream >> citizenID >> virusName;

			if(virusName.empty()){

				if(citizenID.empty()){

					cout << "ERROR.MUST GIVE TWO ARGUMENTS" << endl;
					continue;
				}else if(isWord(citizenID) == false){

					cout << "ERROR.FIRST ARGUMENT MUST BE A DIGIT" << endl;
					continue;
				}

				viruses.vaccineStatus(stoi(citizenID));
			}else{

				if(citizenID.empty() || virusName.empty()){

					cout << "ERROR.MUST GIVE TWO ARGUMENTS." << endl;
					continue;
				
				}else if(isWord(citizenID) == false){

					cout << "ERROR.FIRST ARGUMENT MUST BE A DIGIT" << endl;
					continue;
				}else if(isDate(virusName) == true){

					cout << "ERROR.SECOND ARGUMENT MUST NOT BE A DATE" << endl;
					continue;
				}else if(isWord(virusName) == true){

					cout << "ERROR.SECOND ARGUMENT MUST BE A WORD" << endl;
					continue;
				}

				if( viruses.vaccineStatus(stoi(citizenID),virusName) == false )
					cout << "NOT VACCINATED\n";
			}
		
		}else if(function == "/populationStatus"){

			string country , virusName , date1 , date2;
			inputStream >> country >> virusName >> date1 >> date2;

			if(date2.empty() && date1.empty() && virusName.empty() && country.empty()){

				cout << "ERROR. MUST GIVE 3 ARGUMENTS" << endl;

			}else if(date2.empty() && date1.empty() && virusName.empty()){

				if(isWord(country) == true){

					cout << "ERROR. COUNTRY MUST NOT BE AN INTEGER" << endl;
					continue;

				}else if(isDate(country) == true){

					cout << "ERROR. COUNTRY MUST NOT BE A DATE" << endl;
					continue;
				}

				viruses.populationStatus(&countries,country,lowestDate,highestDate);

			}else if(date2.empty() && date1.empty()){

				if(isWord(country) == true){

					cout << "ERROR. COUNTRY MUST NOT BE AN INTEGER" << endl;
					continue;

				}else if(isDate(country) == true){

					cout << "ERROR. COUNTRY MUST NOT BE A DATE" << endl;
					continue;
				}

				if(isWord(virusName) == true){

					cout << "ERROR. VIRUS NAME MUST NOT BE AN INTEGER" << endl;
					continue;

				}else if(isDate(virusName) == true){

					cout << "ERROR. VIRUS NAME MUST NOT BE A DATE" << endl;
					continue;
				}
				
				viruses.populationStatus(&countries,virusName,lowestDate,highestDate,countries.search(country));
					
			}else if(date2.empty()){

				if(isWord(country) == true){

					cout << "ERROR. VIRUS NAME MUST NOT BE AN INTEGER" << endl;
					continue;

				}else if(isDate(country) == true){

					cout << "ERROR. VIRUS NAME MUST NOT BE A DATE" << endl;
					continue;
				}else if(isDate(virusName) == false){

					cout << "ERROR. DATE 1 MUST BE A DATE" << endl;
					continue;
				}else if(isDate(date1) == false){

					cout << "ERROR. DATE 2 MUST BE A DATE" << endl;
					continue;
				}

				/* Case user gave input without any date but with a country. In that case the function runs with the lowest
				date possible and upper date possible as inputs. */
				if(date1.empty() && date2.empty() && !virusName.empty() && virusName.find('-') != 2){

					date1 = lowestDate;
					date2 = highestDate;
					viruses.populationStatus(&countries,virusName,date1,date2,countries.search(country));
					continue;

				/* Case user gave input without any date. In that case the function runs with the lowest
				date possible and upper date possible as inputs. */
				}else if(virusName.empty()){

					virusName = lowestDate;
					date1 = highestDate;

				/* Case user gave input with only one date. In that case program returns error. */
				}else if( (isDate(date1) == false && isDate(virusName) == false) 
					|| (isDate(country) == false && isDate(virusName) == false)
					|| (isDate(country) == false && isDate(date1) == false) ){
					
					cout << "ERROR. MUST GIVE 2 DATES." << endl;
					continue;
				}

				viruses.populationStatus(&countries,country,virusName,date1);
			}else{

				if(isWord(country) == true){

					cout << "ERROR. COUNTRY MUST NOT BE AN INTEGER" << endl;
					continue;

				}else if(isDate(country) == true){

					cout << "ERROR. COUNTRY MUST NOT BE A DATE" << endl;
					continue;
				}

				if(isWord(virusName) == true){

					cout << "ERROR. VIRUS NAME MUST NOT BE AN INTEGER" << endl;
					continue;

				}else if(isDate(virusName) == true){

					cout << "ERROR. VIRUS NAME MUST NOT BE A DATE" << endl;
					continue;
				}else if(isDate(date1) == false){

					cout << "ERROR. DATE 1 MUST BE A DATE" << endl;
					continue;
				}else if(isDate(date2) == false){

					cout << "ERROR. DATE 2 MUST BE A DATE" << endl;
					continue;
				}

				viruses.populationStatus(&countries,virusName,date1,date2,countries.search(country));
			}

		}else if(function == "/popStatusByAge"){

			string country , virusName , date1 , date2;
			inputStream >> country >> virusName >> date1 >> date2;

			if(date2.empty() && date1.empty() && virusName.empty() && country.empty()){

				cout << "ERROR. MUST GIVE 3 ARGUMENTS" << endl;

			}else if(date2.empty() && date1.empty() && virusName.empty()){

				if(isWord(country) == true){

					cout << "ERROR. COUNTRY MUST NOT BE AN INTEGER" << endl;
					continue;

				}else if(isDate(country) == true){

					cout << "ERROR. COUNTRY MUST NOT BE A DATE" << endl;
					continue;
				}

				viruses.popStatusByAge(&countries,country,lowestDate,highestDate);

			}else if(date2.empty() && date1.empty()){

				if(isWord(country) == true){

					cout << "ERROR. COUNTRY MUST NOT BE AN INTEGER" << endl;
					continue;

				}else if(isDate(country) == true){

					cout << "ERROR. COUNTRY MUST NOT BE A DATE" << endl;
					continue;
				}else if(isWord(virusName) == true){

					cout << "ERROR. VIRUS NAME MUST NOT BE AN INTEGER" << endl;
					continue;

				}else if(isDate(virusName) == true){

					cout << "ERROR. VIRUS NAME MUST NOT BE A DATE" << endl;
					continue;
				}
				
				viruses.popStatusByAge(&countries,virusName,lowestDate,highestDate,countries.search(country));
					
			}else if(date2.empty()){

				if(isWord(country) == true){

					cout << "ERROR. VIRUS NAME MUST NOT BE AN INTEGER" << endl;
					continue;

				}else if(isDate(country) == true){

					cout << "ERROR. VIRUS NAME MUST NOT BE A DATE" << endl;
					continue;
				}else if(isDate(virusName) == false){

					cout << "ERROR. DATE 1 MUST BE A DATE" << endl;
					continue;
				}else if(isDate(date1) == false){

					cout << "ERROR. DATE 2 MUST BE A DATE" << endl;
					continue;
				}

				/* Case user gave input without any date but with a country. In that case the function runs with the lowest
				date possible and upper date possible as inputs. */
				if(date1.empty() && date2.empty() && !virusName.empty() && virusName.find('-') != 2){

					viruses.popStatusByAge(&countries,virusName,lowestDate,highestDate,countries.search(country));
					continue;

				/* Case user gave input without any date. In that case the function runs with the lowest
				date possible and upper date possible as inputs. */
				}else if(virusName.empty()){

					virusName = lowestDate;
					date1 = highestDate;

				/* Case user gave input with only one date. In that case program returns error. */
				}else if( (isDate(date1) == false && isDate(virusName) == false) 
					|| (isDate(country) == false && isDate(virusName) == false)
					|| (isDate(country) == false && isDate(date1) == false) ){
					
					cout << "ERROR. MUST GIVE 2 DATES." << endl;
					continue;
				}

				viruses.popStatusByAge(&countries,country,virusName,date1);
			}else{

				if(isWord(country) == true){

					cout << "ERROR. COUNTRY MUST NOT BE AN INTEGER" << endl;
					continue;

				}else if(isDate(country) == true){

					cout << "ERROR. COUNTRY MUST NOT BE A DATE" << endl;
					continue;
				}else if(isWord(virusName) == true){

					cout << "ERROR. VIRUS NAME MUST NOT BE AN INTEGER" << endl;
					continue;

				}else if(isDate(virusName) == true){

					cout << "ERROR. VIRUS NAME MUST NOT BE A DATE" << endl;
					continue;
				}else if(isDate(date1) == false){

					cout << "ERROR. DATE 1 MUST BE A DATE" << endl;
					continue;
				}else if(isDate(date2) == false){

					cout << "ERROR. DATE 2 MUST BE A DATE" << endl;
					continue;
				}

				viruses.popStatusByAge(&countries,virusName,date1,date2,countries.search(country));
			}

		}else if(function == "/insertCitizenRecord"){

			string citizenID , firstName , lastName , country , age , virusName , info , date;
			inputStream >> citizenID >> firstName >> lastName >> country >> age >> virusName >> info >> date;

			if(date.empty() && info.empty()){

				cout << "ERROR. MUST GIVE 7 ARGUMENTS" << endl;
				continue;
			}else if(isWord(citizenID) == false){

				cout << "ERROR. ID MUST BE AN INTEGER" << endl;
				continue;
			}else if(isDate(firstName) == true){

				cout << "ERROR. FIRST NAME MUST NOT BE A DATE" << endl;
				continue;
			}else if(isWord(firstName) == true){

				cout << "ERROR. FIRST NAME MUST NOT BE AN INTEGER" << endl;
				continue;
			}else if(isDate(lastName) == true){

				cout << "ERROR. LAST NAME MUST NOT BE A DATE" << endl;
				continue;
			}else if(isWord(lastName) == true){

				cout << "ERROR. LAST NAME MUST NOT BE AN INTEGER" << endl;
				continue;
			}else if(isDate(country) == true){

				cout << "ERROR. COUNTRY MUST NOT BE A DATE" << endl;
				continue;
			}else if(isWord(country) == true){

				cout << "ERROR. COUNTRY MUST NOT BE AN INTEGER" << endl;
				continue;
			}else if(isWord(age) == false){

				cout << "ERROR. AGE MUST BE AN INTEGER" << endl;
				continue;
			}else if(isWord(virusName) == true){

				cout << "ERROR. VIRUS NAME MUST NOT BE AN INTEGER" << endl;
				continue;
			}else if(isDate(virusName) == true){

				cout << "ERROR. VIRUS NAME MUST NOT BE A DATE" << endl;
				continue;
			}

			if(date.empty()){

				if(info != "YES" && info != "NO"){

					cout << "ERROR. INFO MUST BE EITHER YES OR NO" << endl;
					continue;
				}

				if(info == "YES"){

					cout << "ERROR. YES MUST BE FOLLOWED BY A DATE" << endl;
					continue;
				}

				viruses.insertCitizenRecord(&citizens,stoi(citizenID),firstNames.insert(firstName),lastNames.insert(lastName),countries.search(country),ages.insert(stoi(age)),virusName,info);
			
			}else{

				if(info != "YES" && info != "NO"){

					cout << "ERROR. INFO MUST BE EITHER YES OR NO" << endl;
					continue;
				}else if(info == "NO"){

					cout << "ERROR. NO MUST NOT BE FOLLOWED BY A DATE" << endl;
					continue;
				}else if(isDate(date) == false){

					cout << "ERROR. DATE MUST BE A DATE" << endl;
					continue;
				}

				viruses.insertCitizenRecord(&citizens,stoi(citizenID),firstNames.insert(firstName),lastNames.insert(lastName),countries.search(country),ages.insert(stoi(age)),virusName,info,dates.insert(date));
			
			}

		}else if(function == "/vaccinateNow"){

			string citizenID , firstName , lastName , country , age , virusName;
			inputStream >> citizenID >> firstName >> lastName >> country >> age >> virusName;

			if(virusName.empty()){

				cout << "ERROR. MUST GIVE 6 ARGUMENTS" << endl;
				continue;
			}else if(isWord(citizenID) == false){

				cout << "ERROR. ID MUST BE AN INTEGER" << endl;
				continue;
			}else if(isDate(firstName) == true){

				cout << "ERROR. FIRST NAME MUST NOT BE A DATE" << endl;
				continue;
			}else if(isWord(firstName) == true){

				cout << "ERROR. FIRST NAME MUST NOT BE AN INTEGER" << endl;
				continue;
			}else if(isDate(lastName) == true){

				cout << "ERROR. LAST NAME MUST NOT BE A DATE" << endl;
				continue;
			}else if(isWord(lastName) == true){

				cout << "ERROR. LAST NAME MUST NOT BE AN INTEGER" << endl;
				continue;
			}else if(isDate(country) == true){

				cout << "ERROR. COUNTRY MUST NOT BE A DATE" << endl;
				continue;
			}else if(isWord(country) == true){

				cout << "ERROR. COUNTRY MUST NOT BE AN INTEGER" << endl;
				continue;
			}else if(isWord(age) == false){

				cout << "ERROR. AGE MUST BE AN INTEGER" << endl;
				continue;
			}else if(isWord(virusName) == true){

				cout << "ERROR. VIRUS NAME MUST NOT BE AN INTEGER" << endl;
				continue;
			}else if(isDate(virusName) == true){

				cout << "ERROR. VIRUS NAME MUST NOT BE A DATE" << endl;
				continue;
			}

			viruses.vaccinateNow(&dates,&citizens,stoi(citizenID),firstNames.insert(firstName),lastNames.insert(lastName),countries.search(country),ages.insert(stoi(age)),virusName);

		}else if(function == "/list-nonVaccinated-Persons"){

			string virusName;
			inputStream >> virusName;

			if(virusName.empty()){

				cout << "ERROR. MUST GIVE 1 ARGUMENT" << endl;
				continue;
			}else if(isDate(virusName) == true){

				cout << "ERROR. VIRUS NAME MUST NOT BE A DATE" << endl;
				continue;
			}else if(isWord(virusName) == true){

				cout << "ERROR. VIRUS NAME MUST NOT BE AN INTEGER" << endl;
				continue;
			}

			viruses.list_nonVaccinated_Persons(virusName);

		}else if(function == "/exit"){

			break;
		}else
			cout << "ERROR.WRONG INPUT" << endl;
	}

	return 0;
}