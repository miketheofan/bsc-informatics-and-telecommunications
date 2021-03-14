#include "types.hpp"

extern int currentYear;
extern int TABLESIZE;

/*Hash Table's constructor initializes a table containing Hash Nodes.*/
HashTable::HashTable(){

	this->table = new HashNode*[TABLESIZE];
	for(int i=0;i<TABLESIZE;i++)
		table[i] = NULL;
}

/*Hash Table's destructor uses deleteList() function in order to delete the whole list of Hash Nodes contained in each position of table*
	and then deletes table* itself.*/
HashTable::~HashTable(){

	for(int i=0;i<TABLESIZE;i++)
		if(this->table[i] != NULL)
			this->table[i]->deleteList();

	delete[] this->table;
}

/*The following function checks if Hash Table is empty.*/
bool HashTable::isEmpty(){

	bool check = true;

	for(int i=0;i<TABLESIZE;i++)
		if(this->table[i] != NULL)
			check = false;

	return check;
}

/*p() function is used to print values of Hash Table.*/
void HashTable::p(){

	cout << "Printing table:" << endl;

	for(int i=0;i<TABLESIZE;i++)

		if(table[i] != NULL){

			HashNode* temp = table[i];
			
			while(temp!=NULL){

				temp->p();
				temp = temp->getNext();
			}

			cout << endl;
		}else
			cout << "EMPTY." << endl;
}

int HashTable::HashFunction(StudentID id){
	return id % TABLESIZE;
}

bool HashTable::i(StudentID id,LastName last,FirstName first,Zip zip,Year year,GPS gps){

	if( this->search(id) != NULL )/*Check if entry already exists and if yes return*/{

		cout << "Student " << id << " already exists." << endl;
		return false;
	}

	int hash = HashFunction(id);/*Calculate in which position of Hash Table this id given must be inserted.*/

	if( table[hash] == NULL ){/*If this position is empty , insert it in the position,othterwise find the end of the list and insert it at the end.*/
		
		cout << "Student " << id << " inserted in Hash Table." << endl;
		table[hash] = new HashNode(id,last,first,zip,year,gps);
	}else{

		HashNode *entry = table[hash];
		while(entry->getNext() != NULL)
			entry = entry->getNext();
		
		cout << "Student " << id << " inserted in Hash Table." << endl;		
		entry->setNext(new HashNode(id,last,first,zip,year,gps));
	}
	return true;
}

HashNode* HashTable::search(StudentID id){

	return this->table[HashFunction(id)] != NULL ? this->table[HashFunction(id)]->search(id) : 0;
	/*If the cell of the Hash Table that corresponds to the id given is not empty,then return the value returned by Hash Node's search() function,
	otherwise return 0(false).*/
}

void HashTable::del(StudentID id){

	this->table[HashFunction(id)] = this->table[HashFunction(id)]->del(id);
	return;
}

/*For every cell of Hash Table use Hash Node's count() functon to count how many items exist in the list and then return the total
amount of items that exist in all lists of cells of the Hash Table.*/
int HashTable::n(Year year){

	int count = 0;

	for(int i=0 ; i< TABLESIZE ; i++){

		if(this->table[i] != NULL) 
			count += this->table[i]->count(year);
	}

	return count;
}

/*The following function runs through every cell of Hash Table and every time stores the max value that exists in the table and inserts
it in a StudentList* that we initialize as NULL in the beginning.Every time a maximum value is found,we check if the student already has been found
as maximum in previous loop and if he exists in returnMax, then we don't do anything(as he is already entried in StudentList*).This whole process will be 
implemented num times , that is as many times as top GPS the user has asked us to find for him.*/
StudentList* HashTable::t(int num,Year year){

	double max;
	StudentList* returnMax = NULL;
	HashNode* maxHash,* tempHash;

	for(int j=0 ; j<num ; j++){

		max = -1;

		for(int i=0 ; i<TABLESIZE ; i++){

			tempHash = this->table[i];
			while(tempHash != NULL){

				if(tempHash != NULL && tempHash->getYear() == year){

					if( tempHash->getGps() > max && returnMax->search(tempHash->getID()) == 0){
						maxHash = tempHash;
						max = tempHash->getGps();
					}
				}
				tempHash = tempHash->getNext();
			}
		}

		if(returnMax == NULL)
			returnMax = new StudentList(maxHash);
		else
			returnMax->i(maxHash);
	}

	return returnMax;
}

/*The following function runs through every cell and its' list and if the entry is registered in year given,he updates a count value by +1(indicating
that another student was found in that year) and also updates sum value with +his GPS.At the end if there was at least one student found we return 
sum/count otherwise we return 0.*/
double HashTable::a(Year year){

	HashNode* tempHash;
	int count =0; 
	double sum = 0;;
	for(int i=0 ; i<TABLESIZE ; i++){

		if(this->table[i] != NULL){

			tempHash = this->table[i];
			while(tempHash != NULL){

				if( tempHash->getYear() == year ){

					count++;
					sum += tempHash->getGps();
				}
				tempHash = tempHash->getNext();
			}
		}
	}

	return count != 0 ? sum/count : 0;
}

/*The following function runs through every cell,list and find the minimum GPS value in Hash Table.Subsequently it runs every cell,list again
in order to find students that have same GPS value as the minimum and then adds them to a StudentList* ,returnMin.So returnMin at the end of the 
function contains every student that has the same GPS as the minimum GPS found in Hash Table.*/
StudentList* HashTable::m(Year year){

	HashNode* tempHash;
	double min;
	StudentList* returnMin = NULL;

	min = 11;

	for(int i=0 ; i<TABLESIZE ; i++){

		if(this->table[i] != NULL){

			tempHash = this->table[i];
			if(tempHash->getYear() == year){

				if( tempHash->getGps() < min)
					min = tempHash->getGps();
			}
			tempHash = tempHash->getNext();
		}
	}

	for(int i=0 ; i<TABLESIZE ; i++){

		if(this->table[i] != NULL){

			tempHash = this->table[i];
			while(tempHash != NULL){

				if(tempHash->getYear() == year){

	/*Source: https://www.geeksforgeeks.org/rounding-floating-point-number-two-decimal-places-c-c/*/
					if( (float)tempHash->getGps()/100 == (float)min/100 ) {/*This method is used to immediately compare double values.*/

						if(returnMin == NULL)
							returnMin = new StudentList(tempHash);
						else
							returnMin->i(tempHash);
					}
				}
				tempHash = tempHash->getNext();
			}
		}
	}

	return returnMin;
}

/*The following function checks if there is even one student registered in year given , inside the Hash Table.*/
bool HashTable::checkForYear(Year year){

	bool check = false;
	for(int i=0; i<TABLESIZE ; i++){

		if(this->table[i] != NULL && this->table[i]->getYear() == year){

			check = true;
			break;
		}
	}

	return check;
}

/*The following function counts how many students are registered to each year that exists in Hash Table.In order to do that,a table is used that has
dimensions equal to the maximum amount of student that can possibly be inserted,and every row of the table contains [year][number of students in year].
Finally,it prints the values of the table.*/
void HashTable::c(){

	int Entries[TABLESIZE][2];

	for(int i=0;i<TABLESIZE;i++)
		Entries[i][0] = -1;

	bool check;

	for(int i=0;i<TABLESIZE;i++){


		if(this->table[i] != NULL){

			check = false;

			for(int j=0;j<TABLESIZE;j++){

				if(Entries[j][0] == currentYear - this->table[i]->getYear()){

					check = true;
					break;
				}
			}

			if(check == false){

				Entries[i][0] = currentYear - this->table[i]->getYear();
				Entries[i][1] = this->n(this->table[i]->getYear());
			}
		}

	}

	for(int i=0;i<TABLESIZE;i++){
		
		if(Entries[i][0] != -1)
			cout << "{Y" << Entries[i][0] << "," << Entries[i][1] << "} ";
	}
}

/*The following function returns the rank^th most populat zip code that exists in Hash Table.In order to do that:*/
void HashTable::postal(int num){

	int Entries[TABLESIZE][2];/*It uses again a table that has dimensions equal to the maximum number of students that can be inserted.Each row this
	time contains [zip code][number of students with this zip code].*/
	int tempI,tempL;
	/*Initialize every row of the table with -1,0*/
	for(int i=0;i<TABLESIZE;i++){

		Entries[i][0] = -1;
		Entries[i][1] = 0;
	}

	bool check;
	int j;
	HashNode* tempHash;

/*Run through every cell,list of Hash Table and insert every individual zip code you find in the table.If zip code already exists,update its' value by +1
otherwise insert the zipcode in table and also update its' value with +1.*/
	for(int i=0;i<TABLESIZE;i++){

		tempHash = this->table[i];

		while(tempHash != NULL){

			check = false;
			
			for(j=0;j<TABLESIZE;j++){

				if(tempHash->getZip() == Entries[j][0]){

					check = true;
					Entries[j][1] += 1;
					break;

				}else if(Entries[j][0] == -1){

					break;				
				}
			}

			if(check == false){

				Entries[j][0] = tempHash->getZip();
				Entries[j][1] += 1;
			}

			tempHash = tempHash->getNext();
		}
	}

/*If the are no students registered in wanted rank , then print error message and return.*/
	if(Entries[num-1][0] == -1){

		cout << "No students are enrolled" << endl;
		return;
	}

/*Otherwise sort array.*/
	for(int i=0;i<TABLESIZE;i++)
		for(int l=i+1;l<TABLESIZE;l++)
			if( Entries[i][1] < Entries[l][1] ){

				tempI = Entries[i][0];
				tempL = Entries[i][1];
				Entries[i][0] = Entries[l][0];
				Entries[i][1] = Entries[l][1];
				Entries[l][0] = tempI;
				Entries[l][1] = tempL;
			}

	int temp = num;
/*And then start printing its values till num of rank wanted.*/
	cout << Entries[temp-1][0] << " ";
	while(Entries[temp-1][1] == Entries[temp][1])
			cout << Entries[temp++][0] << " ";
	cout << "is/are the " << num << " most popular postal code/codes." << endl;

}