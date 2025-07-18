#ifndef _INCL_LOGFILE
#define _INCL_LOGFILE

#include <fstream>
#include "list.hpp"
#include <cstring>

using namespace std;

class Logfile{

private:

	ofstream file;
	int id;
	List* concurrentPeriods;

public:

	Logfile(int);
	~Logfile();
	void write(string);
	void findConcurrent(int);
	void quickFind(string,int);
};

#endif