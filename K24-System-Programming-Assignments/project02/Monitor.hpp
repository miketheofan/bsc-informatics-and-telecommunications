#ifndef _INCL_MONITOR
#define _INCL_MONITOR

#include <iostream>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <poll.h>
#include <signal.h>
#include <string>

using namespace std;

#include "messageProtocol.hpp"
#include "BloomList.hpp"
#include "functions.hpp"
#include "DirectoriesList.hpp"
#include "RecordsList.hpp"
#include "Signals.hpp"

void fill(string,BloomList*);

int expectedArgsChild = 5;
bool searchVaccinationStatus = false;

#endif