#ifndef _INCL_VACCINEMONITOR
#define _INCL_VACCINEMONITOR

#include <fstream>
#include <iostream>
#include <string>
#include <string.h>
#include <sstream>
#include <math.h>

#include "BloomFilter.hpp"
#include "SkipList.hpp"
#include "functions.hpp"
#include "citizensList.hpp"
#include "countriesList.hpp"
#include "agesList.hpp"
#include "stringsList.hpp"

using namespace std;

#define expectedArgs 5
#define probability 0.5
#define numOfFunctions 16
#define lowestDate "01-01-1900"
#define highestDate "30-12-2021"

/* The following number is approximately the total population on earth. */
#define numberOfEntries 7800000000

#endif