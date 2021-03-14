#ifndef _INCL_LIST
#define _INCL_LIST

#include <string>
#include <iostream>

using namespace std;

class List{

private:

	string content;
	List* next;

public:

	List();
	~List();
	void insert(string);
	void print();
};

#endif