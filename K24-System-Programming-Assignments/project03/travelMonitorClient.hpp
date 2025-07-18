#ifndef _INCL_TRAVELMONITOR
#define _INCL_TRAVELMONITOR

#include <iostream>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <cstdio>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sstream>

using namespace std;

#include "monitorServer.hpp"
#include "DirectoriesList.hpp"
#include "messageProtocol.hpp"
#include "BloomFiltersList.hpp"
#include "RequestList.hpp"

#define expectedArgs 13
#define maxFileNameSize 500

int port = 70000;

#endif