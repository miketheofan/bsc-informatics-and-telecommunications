#include "DirectoriesQueue.hpp"

DirectoriesQueue::DirectoriesQueue(int size){

	this->elements = new string[size];
	this->index = -1;
	this->size = size;
}

DirectoriesQueue::~DirectoriesQueue(){
	delete[] this->elements;
}

/* The following function inserts an item to queue. */
void DirectoriesQueue::insert(string item){

	if(this->index == this->size-1)
		return;

	this->elements[++this->index] = item;
}

/* The following function gets current item from queue. */
string DirectoriesQueue::get(){

	if(this->index < 0) return "";

	string tmpString = this->elements[this->index];
	this->elements[this->index] = "";
	index--;

	return tmpString;
}

/* The following function checks whether queue is full or not. */
bool DirectoriesQueue::isFull(){
	return this->index == this->size-1 ? true : false;
}

/* The following function checks whether queue is full or not. */
bool DirectoriesQueue::isEmpty(){
	return this->index < 0 ? true : false;
}