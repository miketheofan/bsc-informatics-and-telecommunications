#ifndef _INCL_DFUNC
#define _INCL_DFUNC

#include <unistd.h>
#include <iostream>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>

using namespace std;

void deleteDirectory(string);
void deleteFile(string);
void checkToDelete(string,string);

extern double totalBytes;
extern int totalCopied;
extern bool verbose;
extern bool deleted;

#endif