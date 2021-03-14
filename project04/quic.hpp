#ifndef _INCL_QUIC
#define _INCL_QUIC

#include "copyFunctions.hpp"
#include "deleteFunctions.hpp"
#include <string>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>
#include <iomanip>

using namespace std;

#define expectedArgc 3
#define defaultPermissions 0777

double totalBytes = 0;
int totalCopied = 0;
bool verbose = false, deleted = false;
time_t currentTime;
int entries = 0;

#endif