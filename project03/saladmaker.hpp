#ifndef _INCL_SALADMAKER
#define _INCL_SALADMAKER

#include <iostream>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <time.h>
#include <fstream>
#include "logfile.hpp"

using namespace std;

#define expectedArgs 9
#define message1 "Waiting for ingredients"
#define message2 "Get Ingredients"
#define message3 "Start making salad"
#define message4 "End making salad"

#endif