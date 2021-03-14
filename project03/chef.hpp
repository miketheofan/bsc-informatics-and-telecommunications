#ifndef _INCL_CHEF
#define _INCL_CHEF

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
#include <fcntl.h>
#include <fstream>
#include "logfile.hpp"

using namespace std;

#define expectedArgs 5
#define IPC_SIZE sizeof(int)*5
#define message1to "Selecting ingredients tomato onion"
#define message1tp "Selecting ingredients tomato pepper"
#define message1po "Selecting ingredients pepper onion"
#define message2a "Notify saladmaker #1"
#define message2b "Notify saladmaker #2"
#define message2c "Notify saladmaker #3"
#define message3 "Man time for resting"

#endif