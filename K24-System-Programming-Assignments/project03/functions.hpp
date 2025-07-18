#ifndef _FUNCTIONS
#define _FUNCTIONS

#include <iostream>
#include <string>
#include <fstream>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>

using namespace std;

#include "Citizen.hpp"
#include "BloomList.hpp"
#include "DirectoriesList.hpp"
#include "messageProtocol.hpp"
#include "CitizensHash.hpp"

unsigned long hash_i(unsigned char *, unsigned int);
void initializeMonitor(int,int*,int*,BloomList**,DirectoriesList**);
void fill(string,BloomList*,DirectoriesList*);
void fillNewDirectory(string,BloomList*);
bool areSame(Citizen*,Citizen*,int,string);
void getMaxFd(int,fd_set*, int*);
int getMonitorByDir(DirectoriesList**,int,string);
bool dateOK(string,string);
bool isLater(string,string);
void fillNewFile(string,BloomList*,citizensList*);

#define noFunctionsFunc 10
#define maximumLevelFunc 10
#define probabilityFunc 0.5

#endif