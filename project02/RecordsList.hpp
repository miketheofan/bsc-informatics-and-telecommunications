#ifndef _INCL_RECORDSLIST
#define _INCL_RECORDSLIST

#include <string>
#include <iostream>

using namespace std;

class RecordsList{

private:

	string content;
	RecordsList *next;

public:

	RecordsList();
	~RecordsList();
	void insert(string);
	RecordsList* getNext();
	void print();
	string getContent();
};

#endif