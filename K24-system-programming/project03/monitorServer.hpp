#ifndef _INCL_MONITOR
#define _INCL_MONITOR

#include <iostream>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>

using namespace std;

#include "messageProtocol.hpp"
#include "DirectoriesList.hpp"
#include "DirectoriesQueue.hpp"
#include "BloomList.hpp"
#include "functions.hpp"
#include "CitizensHash.hpp"

/* Initializing expected arguments as 11 which is the arguments needed for initialization
plus the total amount of countries that exist in the world. */
#define expectedArgsChild 195+11
#define totalCountriesWorldWide 195
#define totalHumansWorldWide 79000
#define noFunctionsFunc 10
#define maximumLevelFunc 10
#define probabilityFunc 0.5

typedef struct {

	string* data;
	int POOL_SIZE;
	int start;
	int end;
	int count;

} pool_t;

pthread_mutex_t mtx;
pthread_mutex_t mutexAvailable;
pthread_mutex_t lockBloomList;

pthread_cond_t  condAvailable;
pthread_cond_t cond_nonempty;
pthread_cond_t cond_nonfull;

pool_t pool;
BloomList *blooms;
DirectoriesQueue *queue;
citizensList * citizens;

int totalFiles;
int numThreads;
int noFiles;

bool terminateThreadsFlag = false;

#endif