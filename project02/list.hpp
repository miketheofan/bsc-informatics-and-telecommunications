#ifndef _INCL_LIST
#define _INCL_LIST

#include "types.hpp"
#include <iostream>
#include <iomanip>

class List{

private:
	
	int number;
	double time;
	List* next;
public:

	List();
	~List();
	int getNumber();
	double getTime();
	List* getNext();
	int getSize();
	void insert(int,double);
	void print();
};

#endif