#include "functions.hpp"

/*
This algorithm (k=33) was first reported by dan bernstein many years 
ago in comp.lang.c. 
The magic of number 33 (why it works better than many other constants, 
prime or not) has never been adequately explained.
*/
unsigned long djb2(unsigned char *str) {
	unsigned long hash = 5381;
	int c; 
	while (c = *str++) {
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	}
	return hash;
}

/*
This algorithm was created for sdbm (a public-domain reimplementation of ndbm) 
database library. it was found to do well in scrambling bits, causing better 
distribution of the keys and fewer splits. it also happens to be a good 
general hashing function with good distribution. The actual function 
is hash(i) = hash(i - 1) * 65599 + str[i]; what is included below 
is the faster version used in gawk. There is even a faster, duff-device 
version. The magic constant 65599 was picked out of thin air while experimenting 
with different constants, and turns out to be a prime. this is one of the 
algorithms used in berkeley db (see sleepycat) and elsewhere.
*/
unsigned long sdbm(unsigned char *str) {
	unsigned long hash = 0;
	int c;

	while (c = *str++) {
		hash = c + (hash << 6) + (hash << 16) - hash;
	}

	return hash;
}

/* 
Return the result of the Kth hash funcation. This function uses djb2 and sdbm.
None of the functions used here is strong for cryptography purposes but they
are good enough for the purpose of the class.

The approach in this function is based on the paper:
https://www.eecs.harvard.edu/~michaelm/postscripts/rsa2008.pdf
*/
unsigned long hash_i(unsigned char *str, unsigned int i) {
	return djb2(str) + i*sdbm(str) + i*i;
}

/* The following function is used in order to fully initialize a monitor. */
void initializeMonitor(int fd,int *bufferSize,int *sizeOfBloom,BloomList** blooms,DirectoriesList** directories){

	/* First we receive a message, that corresponds to the bufferSize that will be sent by parent 
	process, in order for next messages to use. */
	receiveFirstMessage(fd,bufferSize);

	char *bloomString;
	char *directoryString;

	/* Then we receive both sizeOfBloom and directories that this monitor will handle. */
	bloomString = receiveMessage(fd,*bufferSize,'^');
	directoryString = receiveMessage(fd,*bufferSize,'^');

	*sizeOfBloom = stoi(bloomString);

	/* Then we initialize BloomList that was given as argument, using values read in this function. */
	*blooms = new BloomList("0",*sizeOfBloom,noFunctionsFunc,maximumLevelFunc,probabilityFunc);
	/* Then we initialize DirectoriesList that was given as argument, using values read in this function. */
	*directories = new DirectoriesList;

	/* Then we fill both List's with records. */
	fill(directoryString,*blooms,*directories); 

	free(bloomString);
	free(directoryString);
}

/* The following function reads all directories and all their files that are mentioned in directories string,
and fills data structures given as parameters with the records. */ 
void fill(string directories,BloomList* bloomList,DirectoriesList* directoriesList){

	DIR *d;
	struct dirent *dir;

	string mainDir = directories.substr(0,directories.find('/'));
	directories.erase(0,mainDir.length()+1);

	while(!directories.empty()){

		string country = directories.substr(0,directories.find('/'));

		directoriesList->insert(country);

		string currentPath = mainDir+"/"+country;
		
		d = opendir(currentPath.c_str());
		while( (dir = readdir(d)) != NULL ){

			if( !strcmp(dir->d_name,".") || !strcmp(dir->d_name,"..")) continue;

			string filePath = currentPath+"/";
			filePath += dir->d_name;

			ifstream fp;
			fp.open(filePath.c_str());

			string word , arg1 , arg2 , arg3 , arg4 , arg5 , arg6 , arg7 , arg8;
			int count = 1;
			bool flag;

			/* Firstly, the function reads each and every one of the worlds that exist inside the file. */
			while( fp >> word ){

				flag = false;

				/* For every word, depending on its reading turn, we give its value to the corresponding argument variable (arg1,arg2,etc).
				In total we need 8 arguments in order to create a citizen record. */
				switch(count++){

					case(1):
						arg1 = word;
						break;
					case(2):
						arg2 = word;
						break;
					case(3):
						arg3 = word;
						break;
					case(4):
						arg4 = word;
						break;
					case(5):
						arg5 = word;
						break;
					case(6):
						arg6 = word;
						break;
					/*For the 7th argument, if we read "YES" we know that next argument must be a date. If we read "NO",
					we must ensure next argument is not a date, and if it is we print error message and continue to the 
					next record. */
					case(7): 
						arg7 = word;

						if( arg7 == "NO" ) {
							
							fp >> word;

							if(word.find('-') == 2){ /* If next word is a date. */

								cout << "ERROR IN RECORD " << arg1 << " " << arg2 << " " << 
								arg3 << " " << arg4 << " " << arg5 << " " << arg6 << " " << 
								arg7 << " " << word << endl;
							
								count =1; /* Go to first argument read. */
							}else if(word == arg7){
								/* Do nothing. We have reached last word of the file. */
							}else{

								arg8 = emptyDate; /* Otherwise, create new citizen which will have "0" (emptyDate) as date. */

								flag = true; /* This flag is used in order for the execution to enter if statement in line 184,
								even though count is not greater than 7. */

								count =2; /* Since we already read next word because of error handling, next loop will start from second argument. */
							}

						}else{
							
							fp >> word;
							arg8 = word;
						}

						break;
				}

				if( count > 7 || flag == true ){

					if(count>7) count = 1;

					/* A new citizen is created, using pointers to the corresponding structures of every information of his entry. This implementation is used in order to avoid data multiplication.
					Every age,first name,etc is inserted in memory only once and every time it is used again(or even the first time), it is passed in class Citizen's constructor though a pointer. */
					Citizen *c = new Citizen(stoi(arg1),arg2,arg3,arg4,stoi(arg5),arg6,arg8);

					bloomList->insert(c,arg7);
					
					/* This if statement is used to continue next citizen's insertion from second
					argument since we already drew the first word from next line, for error handling reasons. */
					if(flag == true) arg1 = word;
				}
			}
		}

		closedir(d);
			
		directories.erase(0,country.length()+1);
	}
}

/* The following function is used for /addVaccinationRecords function. It does the same functionality as the above one but only for
the directory given as parameter. */
void fillNewDirectory(string directories,BloomList* bloomList){

	DIR *d;
	struct dirent *dir;
	
	d = opendir(directories.c_str());
	while( (dir = readdir(d)) != NULL ){

		if( !strcmp(dir->d_name,".") || !strcmp(dir->d_name,"..")) continue;

		string filePath = directories+"/";
		filePath += dir->d_name;

		ifstream fp;
		fp.open(filePath.c_str());

		string word , arg1 , arg2 , arg3 , arg4 , arg5 , arg6 , arg7 , arg8;
		int count = 1;
		bool flag;

		/* Firstly, the function reads each and every one of the worlds that exist inside the file. */
		while( fp >> word ){

			flag = false;

			/* For every word, depending on its reading turn, we give its value to the corresponding argument variable (arg1,arg2,etc).
			In total we need 8 arguments in order to create a citizen record. */
			switch(count++){

				case(1):
					arg1 = word;
					break;
				case(2):
					arg2 = word;
					break;
				case(3):
					arg3 = word;
					break;
				case(4):
					arg4 = word;
					break;
				case(5):
					arg5 = word;
					break;
				case(6):
					arg6 = word;
					break;
				/*For the 7th argument, if we read "YES" we know that next argument must be a date. If we read "NO",
				we must ensure next argument is not a date, and if it is we print error message and continue to the 
				next record. */
				case(7): 
					arg7 = word;

					if( arg7 == "NO" ) {
						
						fp >> word;

						if(word.find('-') == 2){ /* If next word is a date. */

							cout << "ERROR IN RECORD " << arg1 << " " << arg2 << " " << 
							arg3 << " " << arg4 << " " << arg5 << " " << arg6 << " " << 
							arg7 << " " << word << endl;
						
							count =1; /* Go to first argument read. */
						}else if(word == arg7){
							/* Do nothing. We have reached last word of the file. */
						}else{

							arg8 = emptyDate; /* Otherwise, create new citizen which will have "0" (emptyDate) as date. */

							flag = true; /* This flag is used in order for the execution to enter if statement in line 184,
							even though count is not greater than 7. */

							count =2; /* Since we already read next word because of error handling, next loop will start from second argument. */
						}

					}else{
						
						fp >> word;
						arg8 = word;
					}

					break;
			}

			if( count > 7 || flag == true ){

				if(count>7) count = 1;

				/* A new citizen is created, using pointers to the corresponding structures of every information of his entry. This implementation is used in order to avoid data multiplication.
				Every age,first name,etc is inserted in memory only once and every time it is used again(or even the first time), it is passed in class Citizen's constructor though a pointer. */
				Citizen *c = new Citizen(stoi(arg1),arg2,arg3,arg4,stoi(arg5),arg6,arg8);

				bloomList->insert(c,arg7);
				
				/* This if statement is used to continue next citizen's insertion from second
				argument since we already drew the first word from next line, for error handling reasons. */
				if(flag == true) arg1 = word;
			}
		}			
	}
}

/* The following function inserts all data from a file given to BloomList and citizensList given. */
void fillNewFile(string directories,BloomList* bloomList,citizensList* citizens){

	ifstream fp;
	fp.open(directories);

	string word , arg1 , arg2 , arg3 , arg4 , arg5 , arg6 , arg7 , arg8;
	int count = 1;
	bool flag;

	/* Firstly, the function reads each and every one of the worlds that exist inside the file. */
	while( fp >> word ){

		flag = false;

		/* For every word, depending on its reading turn, we give its value to the corresponding argument variable (arg1,arg2,etc).
		In total we need 8 arguments in order to create a citizen record. */
		switch(count++){

			case(1):
				arg1 = word;
				break;
			case(2):
				arg2 = word;
				break;
			case(3):
				arg3 = word;
				break;
			case(4):
				arg4 = word;
				break;
			case(5):
				arg5 = word;
				break;
			case(6):
				arg6 = word;
				break;
			/*For the 7th argument, if we read "YES" we know that next argument must be a date. If we read "NO",
			we must ensure next argument is not a date, and if it is we print error message and continue to the 
			next record. */
			case(7): 
				arg7 = word;

				if( arg7 == "NO" ) {
					
					fp >> word;

					if(word.find('-') == 2){ /* If next word is a date. */

						cout << "ERROR IN RECORD " << arg1 << " " << arg2 << " " << 
						arg3 << " " << arg4 << " " << arg5 << " " << arg6 << " " << 
						arg7 << " " << word << endl;
					
						count =1; /* Go to first argument read. */
					}else if(word == arg7){
						/* Do nothing. We have reached last word of the file. */
					}else{

						arg8 = emptyDate; /* Otherwise, create new citizen which will have "0" (emptyDate) as date. */

						flag = true; /* This flag is used in order for the execution to enter if statement in line 184,
						even though count is not greater than 7. */

						count =2; /* Since we already read next word because of error handling, next loop will start from second argument. */
					}

				}else{
					
					fp >> word;
					arg8 = word;
				}

				break;
		}

		if( count > 7 || flag == true ){

			if(count>7) count = 1;

			/* A new citizen is created, using pointers to the corresponding structures of every information of his entry. This implementation is used in order to avoid data multiplication.
			Every age,first name,etc is inserted in memory only once and every time it is used again(or even the first time), it is passed in class Citizen's constructor though a pointer. */
			Citizen *c = new Citizen(stoi(arg1),arg2,arg3,arg4,stoi(arg5),arg6,arg8);

			citizens->insert(c);
			bloomList->insert(c,arg7);
			
			/* This if statement is used to continue next citizen's insertion from second
			argument since we already drew the first word from next line, for error handling reasons. */
			if(flag == true) arg1 = word;
		}
	}	

	fp.close();		
}

/* The following function is used to compare two citizens. For my implementation, I count two citizens
as same if they have the same ID and refer to a record for the same virus. That is because we can have multiple
records about a citizen, but for different virus each time. 
Function also checks if there is an entry entered with same id but different name,age,etc. In that case, entry
also counts as same and program print error.*/
bool areSame(Citizen* citizen1,Citizen* citizen2,int i,string info){

	if( (citizen1->getID() == citizen2->getID() && citizen1->getVirusName() == citizen2->getVirusName())
		|| (citizen1->getID() == citizen2->getID() && (citizen1->getFirstName() != citizen2->getFirstName() 
			|| citizen1->getLastName() != citizen2->getLastName() || citizen1->getAge() != citizen2->getAge() 
			|| citizen1->getCountry() != citizen2->getCountry()) ) ){

		if(i == 0){

			cout << "ERROR IN RECORD ";
			citizen1->print(info);

		}else
			cout <<"ERROR: ID ALREADY EXISTS IN ENTRY WITH DIFFERENT DATA" << endl;

		return true;
	}

	return false;
}

/* The following function inserts max file descriptor to max_fd argument that is given. */
void getMaxFd(int fd, fd_set *fds, int *max_fd) {

    FD_SET(fd, fds);
    if(fd > *max_fd) *max_fd = fd;

}

/* The following function returns which monitor handles data for country given. */
int getMonitorByDir(DirectoriesList** dl,int size,string country){

	for(int i=0;i<size;i++)
		if(dl[i]->countryExists(country))
			return i;

	return -1;
}

/* The following function checks if request travel date is six or more months after
vaccination date. */
bool dateOK(string vaccinated,string request){

	int dayVaccinated = stoi(vaccinated.substr(0,vaccinated.find("-")));
	vaccinated.erase(0,3);
	int monthVaccinated = stoi(vaccinated.substr(0,vaccinated.find("-")));
	vaccinated.erase(0,3);
	int yearVaccinated = stoi(vaccinated);

	int dayRequested = stoi(request.substr(0,request.find("-")));
	request.erase(0,3);
	int monthRequested = stoi(request.substr(0,request.find("-")));
	request.erase(0,3);
	int yearRequested = stoi(request);

	if(yearRequested > yearVaccinated)
		return true;
	else if(yearRequested < yearVaccinated)
		return false;
	else{

		if(monthRequested < monthVaccinated)
			return false;
		else if(monthRequested > monthVaccinated + 6)
			return true;
		else if(monthRequested == monthVaccinated)
			if(dayRequested >= dayVaccinated )
				return true;
			else 
				return false;
		else 
			return false;
	}
}

/* The following function compares two dates in string format. 
The result corresponds to: is date1 later than date2? */
bool isLater(string date1,string date2){

	int day1 = stoi(date1.substr(0,date1.find("-")));
	date1.erase(0,3);
	int month1 = stoi(date1.substr(0,date1.find("-")));
	date1.erase(0,3);
	int year1 = stoi(date1);

	int day2 = stoi(date2.substr(0,date2.find("-")));
	date2.erase(0,3);
	int month2 = stoi(date2.substr(0,date2.find("-")));
	date2.erase(0,3);
	int year2 = stoi(date2);

	if(year1 < year2) /* If date1's year is lesser than date2's one,
				then return false. */
		return false;
	else if(year1 > year2) /* Otherwise return true. */
		return true;

	if(month1 < month2) /* If years are equal, do the same thing but this time for months. */
		return false;
	else if(month1 > month2)
		return true;

	if(day1 < day2) /* If months are equal too, do the same thing for days. */
		return false;
	else if(day1 > day2)
		return true;

	/* Default return value is set to true, as when we will need to check if a date is inside a span, lower
	and upper bound values count as inside span. */ 
	return true;
}