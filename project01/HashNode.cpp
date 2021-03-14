#include "types.hpp"

extern int currentYear;
extern int TABLESIZE;

HashNode::HashNode(StudentID id,LastName last,FirstName first,Zip zip,Year year,GPS gps){

	this->id = id;
	this->last = last;
	this->first = first;
	this->zip = zip;
	this->year = year;
	this->gps = gps;
	this->next = NULL;
}

void HashNode::p(){

	cout << "[" << this->id << "," << this->last << "," << this->first << "," << this->zip 
	<< "," << this->year << "," << this->gps << "]";
}

int HashNode::getID(){
	return this->id;
}

string HashNode::getLast(){
	return this->last;
}

string HashNode::getFirst(){
	return this->first;
}

int HashNode::getZip(){
	return this->zip;
}

int HashNode::getYear(){
	return this->year;
}

double HashNode::getGps(){
	return this->gps;
}

HashNode* HashNode::getNext(){
	return this->next;
}

void HashNode::setNext(HashNode* next){

	this->next = next;
}

/*The following function inserts a HashNode given as the next node of current node.If there already exists a next node , it deletes it and replace it 
with the given one.*/
void HashNode::insertNext(HashNode* next){

	if(this->next != NULL)
		delete this->next;
		
	this->next = new HashNode(next->getID(),next->getLast(),next->getFirst(),next->getZip(),next->getYear(),next->getGps());
}

/*The following function searches for student with id equal to the one given in current Hash Node and its next nodes.*/
HashNode* HashNode::search(StudentID id){

	if(this->id == id)
		return this;
	else if(this->next == NULL)
		return 0;
	else
		return this->next->search(id);

}

/*The following function deletes the node that need to be deleted and returns the list as it was before just with the
certain node missing.*/
HashNode* HashNode::del(StudentID id){

	HashNode *prev = this,* current = this->next;

	if(this->id == id){/*If we have reached the node containing the id we need to delete,delete certain node and return next node.*/

		delete prev;

		return current;
	}

	while( current != NULL ){/*Go through every list's node and if the id we are searching for is found ,stop traversing.*/

		if(current->getID() == id)
			break;
		else{
			prev = current;
			current = current->getNext();
		}
	}

	prev->setNext(current->getNext());/*Delete the node we found with the corresponding id and return the list.*/

	delete current;
	
	return this;

}

/*The following function calculates how many students exist in Hash Node's list with registered year equal to the year given.*/
int HashNode::count(Year year){

	if( this->year == year )

		if(this->next != NULL)
			return 1 + this->next->count(year);
		else
			return 1;
	else

		if(this->next != NULL)
			return 0 + this->next->count(year);
		else 
			return 0;
}

/*The following function checks if Hash Node's zip code is equal to zip code given.*/
int HashNode::getZipCount(Zip zip){
	return this->zip == zip ? 1 : 0;
}

/*The following function fully deletes the Student List contained in HashNode and the ones contained in its next nodes, 
without deleting its HashNode* student.*/
void HashNode::deleteList(){

	if(this->next != NULL)
		this->next->deleteList();

	delete this;
}