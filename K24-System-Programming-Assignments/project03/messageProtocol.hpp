#ifndef _INCL_PROTOCOL
#define _INCL_PROTOCOL

#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <cstdio>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define firstMessageBuffer sizeof(int)

using namespace std;

void sendFirstMessage(int,int);
void sendMessage(int,int,char*,int,char*);
void receiveFirstMessage(int,int*);
void sendBloom(int,int,char*,int,string);
char* receiveMessage(int,int,char);

#endif
