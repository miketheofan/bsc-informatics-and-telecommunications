#ifndef _INCL_CFUNC
#define _INCL_CFUNC

#include <iostream>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>

using namespace std;

void deepCopy(string,string,string);
void quicFunction(string,string);
bool checkToFind(string,string);
void copyFile(string,string);
bool checkCriteria(string,string);

extern double totalBytes;
extern int totalCopied;
extern bool verbose;
extern bool deleted;
extern time_t currentTime;
extern int entries;

#endif