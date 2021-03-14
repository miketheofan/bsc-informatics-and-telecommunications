#include "types.hpp"
#include "list.hpp"

List::List(){

	this->number = -2;
	this->time = -2;
	this->next = NULL;
}

List::~List(){

	if(this->next != NULL)
		delete this->next;
}

int List::getNumber(){
	return this->number;
}

double List::getTime(){
	return this->time;
}

List* List::getNext(){
	return this->next;
}

int List::getSize(){
	
	int size = 1;
	List* temp = this->next;

	while(temp != NULL){

		temp = temp->getNext();
		size++;
	}

	return size;
}

void List::insert(int number,double time){

	if(this->number == -2){/*If list node is empty, enter data in current node and return*/

		this->number = number;
		this->time = time;
		return;
	}

	if(this->number == number && this->number != -1) /*Checks if number already exists and if node is a total time of some W node ( in which case will have -1 as number and correct time).*/
		return;

	if(this->next == NULL)
		this->next = new List;

	this->next->insert(number,time);

}

void List::print(){

	cout << "Number: " << this->number << "\tTime: " << setprecision(4) << this->time << endl;

	if(this->next != NULL)
		this->next->print();
}