#ifndef _INCL_TRAVELMONITOR
#define _INCL_TRAVELMONITOR

#include <iostream>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sstream>
#include <signal.h>

using namespace std;

#include "Monitor.hpp"
#include "BloomFilter.hpp"
#include "messageProtocol.hpp"
#include "BloomFiltersList.hpp"
#include "RequestList.hpp"
#include "Signals.hpp"

int expectedArgsParent = 9;
int maxFileNameSize = 500;

#endif