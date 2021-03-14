#include "types.hpp"

extern int currentYear;
extern int TABLESIZE;

StudentList::StudentList(HashNode* student){

	this->student = student;
	this->next = NULL;
}

/*Destructor of class StudentList deletes the HashNode* that is contained.*/
StudentList::~StudentList(){

	delete this->student;
}

StudentList* StudentList::getNext(){
	return this->next;
}

void StudentList::setNext(StudentList* next){
	this->next = next;
}

HashNode* StudentList::getStudent(){
	return this->student;
}

void StudentList::setStudent(HashNode* student){
	this->student = student;
}

/*The following class prints the items of the list.*/
void StudentList::p(){

	this->student->p();

	if(this->next != NULL){
		cout << endl;
		this->next->p();
	}
}

/*The following function checks if there is a next node of current node in list and if no it inserts the given student,otherwise it inserts
new node as the second node of the list and shifts the rest by one to the right.*/
void StudentList::i(HashNode* student){

	if(this->student != NULL){

		if(this->next == NULL)
			this->next = new StudentList(student);
		else{

			StudentList* nextPointer = this->next;
			this->next = new StudentList(student);
			this->next->setNext(nextPointer);
		}
	}else

		this->student=student;
}

/*The following function deletes the node that need to be deleted and returns the list as it was before just with the
certain node missing.*/
StudentList* StudentList::del(StudentID id){

	StudentList *prev = this,* current = this->getNext();

	if(this->student->getID() == id){/*If we have reached the node containing the id we need to delete,delete certain node and return next node.*/

		delete prev;

		return current;
	}

	while( current != NULL ){/*Go through every list's node and if the id we are searching for is found ,stop traversing.*/

		if(current->student->getID() == id)
			break;
		else{
			prev = current;
			current = current->getNext();
		}
	}

	if(current == NULL) /*If we have reached the end of the list return the list just as it was.*/
		return this;

	prev->setNext(current->getNext());/*Otherwise,delete the node we found with the corresponding id and return the list.*/

	delete current;
	
	return this;

}

/*The following list counts the numbers students contained in students list.*/
int StudentList::count(){

	StudentList* tempList = this;
	int count = 0;

	while(tempList != NULL){

		count++;
		tempList = tempList -> getNext();
	}

	return count;
}

/*The following function finds the top num students based on their GPS.*/
/*It traverses the whole list num times , and every time finds the top GPS that isn't already inserted in StudentList* returnMax.*/
/*So at the end of the function the top num times will be indivudually inserted.*/
StudentList* StudentList::t(int num,Year year){

	double max;
	HashNode* maxHash , *tempHash;
	StudentList* tempStudent;
	StudentList *returnMax = NULL;

	for(int i=0 ; i<num ; i++){

		maxHash = this->student;
		max = -1;
		tempStudent = this;

		while(tempStudent != NULL){

			tempHash = tempStudent->getStudent();

			if(tempHash->getGps() > max && returnMax->search(tempHash->getID()) == 0){
				maxHash = tempHash;
				max = tempHash->getGps();
			}	

			tempStudent = tempStudent->getNext();
		}

		if(returnMax == NULL)
			returnMax = new StudentList(maxHash);
		else
			returnMax->i(maxHash);
	}

	return returnMax;
}

/*The following function searches for a student with id equal to the id given.*/
HashNode* StudentList::search(StudentID id){

	if(!this)
		return 0;

	if(this->student->getID() == id)
		return this->student;
	else
		if(this->next != NULL)
			return this->next->search(id);
		else
			return 0;
}

/*The following function returns the average GPS of the students contained in Students List , by recursively getting the total amount of GPS contained 
in the current node and it's next one and if there isn't one returns only the current one's gps.*/
double StudentList::a(){
	return this->next != NULL ? this->student->getGps() + this->next->a() : this->student->getGps();
}

/*The following function returns the students with the minimum GPS,by finding first of all the minimum GPS contained in the list and then
finding all students that have GPS equal to the minimum one.It inserts those students in StudentList* returnMin and returns it.*/ 
StudentList* StudentList::m(Year year){

	double min;
	HashNode *tempHash;
	StudentList* tempStudent;
	StudentList *returnMin = NULL;

	min = 11;
	tempStudent = this;

	while(tempStudent != NULL){
		tempHash = tempStudent->getStudent();

		if(tempHash->getGps() < min){
			min = tempHash->getGps();
		}	

		tempStudent = tempStudent->getNext();
	}

	tempStudent = this;

	while(tempStudent != NULL){

		tempHash = tempStudent->getStudent();

		if( (float)tempHash->getGps()/100 == (float)min/100){/*This method is used to immediately compare double values.*/

			if(returnMin == NULL)
				returnMin = new StudentList(tempHash);
			else
				returnMin->i(tempHash);
		}

		tempStudent = tempStudent->getNext();
	}

	return returnMin;

}

/*The following function fully deletes a Student List without deleting its HashNode* student.*/
void StudentList::deleteList(){

	if(this->next != NULL)
		this->next->deleteList();

	delete this;
}