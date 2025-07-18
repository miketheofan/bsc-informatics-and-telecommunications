#include "types.hpp"

extern int currentYear;
extern int TABLESIZE;

/* Source: http://www.cplusplus.com/forum/beginner/32329/ */
int getCurrentYear(){

	time_t temp = time(NULL);
	tm* timePtr = localtime(&temp);
	
	int ret =  timePtr->tm_year + 1900;
	return ret;
}

/* Source: https://stackoverflow.com/questions/3482064/counting-the-number-of-lines-in-a-text-file */
int getFileSize(string fileName){

	int number_of_lines = 0;
    string line;
    ifstream myfile(fileName);

    if(myfile.is_open() == false)
    	return -1;

    while (getline(myfile, line))/*while there is another line,add one to the value that is goind to be returned.*/
        number_of_lines++;

    myfile.close();

    return number_of_lines;
}

/*The following function fills both Hash Table and Inverted Index with the content of a text file given.*/
void fill(HashTable* hash,InvertedIndex* list,string fileName){

	HashNode* tempHash;
	bool check;
	string word,arg1,arg2,arg3,arg4,arg5,arg6;
	int count = 1;
	ifstream fp; 
	fp.open(fileName);/*Open file,*/

	while( fp >> word){/*while there is another word in the file,*/

		switch(count++){/*count valuable is being used to know whether a word is the 1st we read or the 2nd we read etc.*/ 

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
		}

		if(count > 6){/*If we have read more than 6 words,we have everything we need to fill another node.*/
			
			count = 1;/*count goes again to 1 so that the process continues for next line,*/
			check = hash->i(stoi(arg1),arg2,arg3,stoi(arg4),stoi(arg5),stof(arg6));/*and then use the i(nsert) function to pass the arguments we have just read from the file in hash table,inverted index.
																			Stoi and stof are functions that belong to <string> library and are used to cast the string value that was read 
																	from the file as an integer and float respectively because of i(nsert)'s argument standards.*/
			if(check == true){/*If node's id read already exists in hash table,don't insert it to inverted index.*/
							/*We use hash table's search here to check for duplicates because it is faster.*/
				tempHash = new HashNode(stoi(arg1),arg2,arg3,stoi(arg4),stoi(arg5),stof(arg6));
				list->i(tempHash,currentYear - stoi(arg5));
			}
			continue;
		}

	}
	fp.close();/*close the file*/
}

bool l(HashTable* ht,InvertedIndex* ii,StudentID id){

	// HashNode* tempHash = ht->search(id);
	HashNode* tempHash = ii->search(id);/*call Inverted Index's search() function in order to chech if id given exists in our Inverted Index.*/
	if( tempHash == 0 ){/*If function returns 0 it means that no student with id given was found.So print a message and then return false.*/
		
		cout << "Student " << id << " does not exist." << endl;
		return false;

	}else tempHash->p();/*Otherwise,print the Hash Node's content.*/
	cout << endl;
	return true;/*If everything worked fine,return true.*/
}

void i(HashTable* ht,InvertedIndex* ii,StudentID id,LastName last,FirstName first,Zip zip,Year year,GPS gps){

	if( l(ht,ii,id) == true ){ /*Look up for a student with id equal to the one given,and if there exists one , print the message that
								student already exists and return.*/

		cout << "Student " << id << " exists." << endl;
		return;
	}
	ht->i(id,last,first,zip,year,gps);/*Otherwise,insert student in our Hash Table using the arguments given.*/
	HashNode *tempHash = new HashNode(id,last,first,zip,year,gps);
	ii->i(tempHash,currentYear - year);/*Also insert him in out Inverted Index using the arguments given.*/
	
	cout << "Student " << id << " inserted." << endl;

	return;
}

void d(HashTable* ht,InvertedIndex* ii,StudentID id){

	if( l(ht,ii,id) == false ) /*If there is no student with that id , return.*/
		return;

	ht->del(id);/*Other wise delete the student both from out Hash Table and our Inverted Index.*/
	ii->del(id);
	cout << "Record " << id << " deleted." << endl;
	return;
}

int n(HashTable* ht,InvertedIndex* ii,Year year){
	// return ht->n(year);
	return ii->n(year);
}

void t(HashTable* ht,InvertedIndex* ii,int num,Year year){

	StudentList * sl = NULL;

	if(ii->checkForYear(year) == false){/*Using function checkForYear() we check if there is no List of students registered in the year given.*/

		cout << "No students enrolled in " << year << endl;
		return;
	}

	if( n(ht,ii,year) < num ){/*Otherwise,check if the number of students that the user wants us to return is more than the number of students that
								actually are registered in this year and if yes call t() function using the total amount of students registered.*/ 

		sl = ii->t(n(ht,ii,year),year);
		//sl = ht->t(n(ht,ii,year),year);
	}else{/*Otherwise,use the t() function with the number that the user passed as an argument.*/

		sl = ii->t(num,year);
		//sl = ht->t(num,year);
	}

	StudentList* tempList = sl;
	while(tempList != NULL){

		cout << "{" << tempList->getStudent()->getID() << "} ";
		tempList = tempList->getNext();
	}
	cout << endl;

	StudentList *next;

/*Delete the pointer to StudentList class we created,but careful not to delete the actual values that the pointer contains.Because that might affect some of the items in our structures.*/
	while(sl != NULL){

		next = sl->getNext();
		HashNode* tempHash = NULL;
		sl->setStudent(tempHash); //Doing that for the Hash Node's value not to be deleted.
		delete sl;	
		sl=next;
	}
}

void a(HashTable* ht,InvertedIndex* ii,Year year){

	// double temp = ht->a(year);
	double temp = ii->a(year);/*Call a() function for out Inverted Index.*/
	if(!temp)/*If a() function returned 0 it means there are no students registered in the year given.*/
		cout << "No students enrolled in " << year << endl;
	else
		cout << temp << endl;
}

void m(HashTable* ht,InvertedIndex* ii,Year year){

	if(ii->checkForYear(year) == false){/*Check if students registered in year given exist in our structure.*/

		cout << "No students enrolled in " << year << endl;
		return;
	}

	StudentList* min = ii->m(year);
//	StudentList* min = ht->m(year);

	StudentList* tempList = min;
	while(tempList != NULL){
		
		cout << "{" << tempList->getStudent()->getID() << "} ";
		tempList = tempList->getNext();
	}
	cout << endl;

	StudentList *next;

/*Same process for deleting StudentList* and not actually effect our whole structure.*/
	while(min != NULL){

		next = min->getNext();
		HashNode* tempHash = NULL;
		min->setStudent(tempHash);//Doing that for the HashNode's value not to be deleted.
		delete min;
		min = next;
	}
}

void c(HashTable* ht,InvertedIndex* ii){

	if(ht == NULL || ii == NULL){/*If our hash table and our inverted index are empty,then print error message and return.*/
		
		cout << "No students are enrolled." << endl;
		return;
	}

//	ht->c();
	ii->c();
	cout << endl;
}

void p(HashTable* ht,InvertedIndex* ii,int num){

//	ii->postal(num);
	ht->postal(num);/*Call postal() function.*/
}