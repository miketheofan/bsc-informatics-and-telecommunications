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

/* The following is a function that checks if string given is a word or a number. */
bool isWord(string s){

	for(unsigned int i = 0; i < s.length(); i++)
        if(isdigit(s[i]) == false)
            return false;
 
    return true;
}

/* The following is a function that checks if string given is a date. To do that, it takes advantage of the date format that is
used in my implementation: DD-MM-YYYY. */
bool isDate(string s){

	if(s.find('-') == 2) return true;
	return false;
}

/* The following function compares two dates in string format. 
The result corresponds to: is date1 later than date2? */
bool compare(string date1,string date2){

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

/*The following function "fills" the data structures we need for this project. As arguments, it takes a records file as a string, a pointer to 
a List object, which we need to fill, a pointer to a citizens list in order to check for duplicate records and also to fill the list and a countries list which 
we also need to fill. */
void fill(string fileName, virusesList *list, citizensList *citizens,countriesList* countries,stringsList* firstNames,stringsList* lastNames,agesList* ages,stringsList* dates){

	ifstream fp;
	fp.open(fileName);
	string word , arg1 , arg2 , arg3 , arg4 , arg5 , arg6 , arg7 , arg8;
	int count = 1;
	virusesList *viruses = list;
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
			Citizen *c = new Citizen(stoi(arg1),firstNames->insert(arg2),lastNames->insert(arg3),countries->search(arg4),ages->insert(stoi(arg5)),viruses->insertVirus(arg6),dates->insert(arg8));

			if(citizens->insert(c,0) == true) /* If record doesn't exist in data structure or it isn't an ERROR entry, insert the record to the List strucutre. */
				viruses->insertCitizen(c,arg7);
			
			/* This if statement is used to continue next citizen's insertion from second
			argument since we already drew the first word from next line, for error handling reasons. */
			if(flag == true) arg1 = word;
		}
	}
}