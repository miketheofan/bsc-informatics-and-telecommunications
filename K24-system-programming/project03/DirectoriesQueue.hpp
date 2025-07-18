#ifndef _DIRECTORIESQUEUE
#define _DIRECTORIESQUEUE

#include <string>
#include <iostream>

using namespace std;

class DirectoriesQueue{

private:

	string *elements;
	int index;
 	int size;

public:

	DirectoriesQueue(int);
	~DirectoriesQueue();
	void insert(string);
	string get();
	bool isFull();
	bool isEmpty();

};

#endif