#ifndef _INCL_FUNCTIONS
#define _INCL_FUNCTIONS

#include <stdio.h>
#include <string>
#include <fstream>

#include "SkipList.hpp"
#include "BloomFilter.hpp"
#include "Citizens.hpp"
#include "virusesList.hpp"
#include "citizensList.hpp"
#include "countriesList.hpp"
#include "stringsList.hpp"
#include "agesList.hpp"

unsigned long hash_i(unsigned char *, unsigned int);
void fill(string,virusesList*,citizensList*,countriesList*,stringsList*,stringsList*,agesList*,stringsList*);
bool isWord(string);
bool isDate(string);
bool compare(string,string);
void vaccineStatusBloom(citizenID,string);

#endif