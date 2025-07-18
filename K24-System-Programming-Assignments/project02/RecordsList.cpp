#include "RecordsList.hpp"

RecordsList::RecordsList(){

	this->content = " ";
	this->next = NULL;
}

RecordsList::~RecordsList(){

	if(this->next != NULL) delete this->next;
}

/* The following function inserts a record in RecordsList. */
void RecordsList::insert(string content){

	if(this->content == " "){

		this->content = content;
		return;
	}

	if(this->next == NULL)
		this->next = new RecordsList;

	this->next->insert(content);
}

RecordsList* RecordsList::getNext(){
	return this->next;
}

void RecordsList::print(){
	cout << this->content << endl;
	if(this->next != NULL)
		this->next->print();
}

string RecordsList::getContent(){
	return this->content;
}