#ifndef _INCL_SIGNALS
#define _INCL_SIGNALS

#include <signal.h>
#include <iostream>
#include <wait.h>

using namespace std;

void mySignal(int,siginfo_t *,void *);
void sigChild(int);
void sigintMonitor(int);
void sigquitMonitor(int);
void sigintTravel(int);
void sigquitTravel(int);

extern bool addVaccinationRecords;
extern int childKilled;
extern bool sigIntMonitor;
extern bool sigQuitMonitor;
extern bool sigIntTravel;
extern bool sigQuitTravel;

#endif