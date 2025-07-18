#include "list.hpp"

List::List(){

	this->content = "0";
	this->next = NULL;
}

List::~List(){

	if(this->next != NULL) delete this->next;
}

/* Following function inserts a string to the list. */
void List::insert(string content){

	if(this->content == "0"){

		this->content = content;
		return;
	}

	if(this->next == NULL) {

		this->next = new List;
		this->insert(content);
	}else 
		this->next->insert(content);
}

/* Following function prints the contents of the list. */
void List::print(){

	cout << content << endl;
	if(this->next != NULL) this->next->print();
}