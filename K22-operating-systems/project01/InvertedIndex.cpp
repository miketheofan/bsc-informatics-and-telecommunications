#include "types.hpp"

extern int currentYear;
extern int TABLESIZE;

InvertedIndex::InvertedIndex(){

	this->Y = EMPTY;
	this->next = NULL;
	this->student = NULL;
}

/*Inverted Index's destructor deletes the Student List that exists inside of the current InvertedIndex and the following of the list.*/
InvertedIndex::~InvertedIndex(){

	/*If list is empty then don't do anything.*/
	if(this->isEmpty() == true)
		return;

	if(this->next != NULL)
		delete this->next;
	
	if(this->student != NULL)
		this->student->deleteList();
}

/*The following function checks if Inverted Index is empty.*/
bool InvertedIndex::isEmpty(){

	if(this->Y == EMPTY)
		return true;
	return false;
}

int InvertedIndex::getY(){
	return this->Y;
}

InvertedIndex* InvertedIndex::getNext(){
	return this->next;
}

void InvertedIndex::i(HashNode* student,Year Y){

	if(this->Y == EMPTY)

		this->Y = Y;

	if( student->getYear() != currentYear-this->Y ){/*If this list node is corresponding to the year given,insert it to this list,otherwise call
		i() for the next node of the Inverted Index list.If there isn't one create next node with HashNode* student given.*/
		
		if(this->next != NULL)

			this->next->i(student,currentYear-this->Y);
		else{

			this->next = new InvertedIndex();
			this->next->i(student,currentYear - student->getYear());
		}

	}else{

		if(this->student == NULL){

			this->student = new StudentList(student);
			cout << "Student " << student->getID() << " inserted in Inverted Index." << endl;
		}else{

			this->student->i(student);
			cout << "Student " << student->getID() << " inserted in Inverted Index." << endl;
		}

	}
}

StudentList* InvertedIndex::getStudent(){
	return this->student;
}

/*The following function prints the items that exist in the Inverted Index list.*/
void InvertedIndex::p(){

	cout << "Y" << this->Y << endl;

	if(this->student == NULL){

		cout << "There are no students here." << endl;
		return;
	}

	this->student->p();

	cout << endl << endl;;

	if(this->next != NULL)
		this->next->p();
}

/*If current node contains a student check if the to be deleted student is contained in there , otherwise continue to next list node.*/
void InvertedIndex::del(StudentID id){

	if(this->student != NULL)
		this->student = this->student->del(id);

	if( this->next != NULL )
		this->next->del(id);

	return;
}

/*The following function uses c() function to find the count of students registered in year given of every node's list of students and keeps doing as
long as next nodes exist.*/
int InvertedIndex::n(Year year){

	if(this->Y != currentYear - year){
		if(this->next != NULL)
			this->next->n(year);
	}
	else if(this->Y == currentYear - year)
		return this->student->count();
	else
		if(this->next == NULL)
			return 0;
		else
			this->next->n(year);
}

/*The following functin uses Student List's search() function in order to check if student with given id exists in students list.*/
HashNode* InvertedIndex::search(StudentID id){

	if(this->student->search(id) != 0)
		return this->student->search(id);
	else
		if(this->next != NULL)
			return this->next->search(id);
	return 0;
}

/*The following function uses Student List's t() function in order to find top num students based on registered GPS.*/
StudentList* InvertedIndex::t(int num,Year year){

	StudentList* tempHash = NULL;

	if(this->Y == currentYear - year)
		tempHash = this->student->t(num,year);
	else
		if(this->next != NULL)
			tempHash = this->next->t(num,year);

	return tempHash;
}

/*The following function uses Student List's count() and a() functions , which return number of students that were registered in year given and exist
in Student List and total amount of GPS summed from students that are registered in year given.Finally it returns total / count if we are in the
list node corresponding to the correct year otherwise it continues to next list node.*/
double InvertedIndex::a(Year year){

	if(this->Y == currentYear - year)
		return this->student->a() / this->student->count();
	else{
		if(this->next != NULL)
			return this->next->a(year);
		return 0;
	}

}

/*The following function uses Student List's m() function in order to find minimum student's GPS that exist in students list.If we are in the list's node
with the correct year it calls the function ,otherwise it continues in next node.*/
StudentList* InvertedIndex::m(Year year){

	if(this->Y == currentYear - year)
		return this->student->m(year);
	else
		if(this->next != NULL)
			return this->next->m(year);
		return 0;
}

/*The following function checks if there is a list node corresponding to the year given.*/
bool InvertedIndex::checkForYear(Year year){

	if(this->getY() == currentYear - year)
		return true;
	else{
		
		if(this->next != NULL)
			return this->next->checkForYear(year);
		return false;
	}
}

/*The following function counts the total amount of students registered in current list's node year.*/
void InvertedIndex::c(){

	if(this->student->count() != 0)
		cout << "{Y" << this->Y << "," << this->student->count() << "} ";
	if(this->next != NULL)
		this->next->c();
	return;
}

/*The following function returns the rank^th most populat zip code that exists in Inverted Index list using the same method as the postal() function
of HashTable class.*/
void InvertedIndex::postal(int num){

	int Entries[TABLESIZE][2];
	int tempI,tempL;
	for(int i=0;i<TABLESIZE;i++){/*It uses a 2D table that is being initialized the same way as the one in HashTable class was.*/

		Entries[i][0] = -1;
		Entries[i][1] = 0;
	}

	bool check;
	int j;
	InvertedIndex * tempInv = this;
	StudentList *tempList;
	HashNode* tempHash;

/*Also finds all zip codes contained in list and stores them the same way as it did in HashTable class one.*/
	/*[zipcode][no of students with zip code]*/
	while(tempInv != NULL){

		tempList = tempInv->getStudent();

		while(tempList != NULL){

			tempHash = tempList->getStudent();

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

			tempList = tempList->getNext();
		}

		tempInv = tempInv->getNext();
	}

/*checks if rank does not exist in entries in the same way.*/
	if(Entries[num-1][0] == -1){

		cout << "No students are enrolled." << endl;
		return;
	}

/*Sorting values in table.*/
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