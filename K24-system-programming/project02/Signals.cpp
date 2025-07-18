#include "Signals.hpp"

/* The following function is used from travelMonitor to raise a SIGUSR1 signal. */
void mySignal(int signalNumber,siginfo_t *signalInfo,void *context){

	addVaccinationRecords = true;
}

/* The following function is used to raise a SIGCHILD signal. */
void sigChild(int signalNumber){

	pid_t pid;
	/* We use wait here in order to know which child process has terminated.
	This information is useful for travelMonitor. */
	pid = wait(NULL);

	childKilled = pid;
}

/* The following function is used to handle SIGINT signal in travelMonitor process. */
void sigintMonitor(int signalNumber){
	sigIntMonitor = true;
}

/* The following function is used to handle SIGQUIT signal in Monitor process. */
void sigquitMonitor(int signalNumber){
	sigQuitMonitor = true;
}

/* The following function is used to handle SIGINT signal in travelMonitor process. */
void sigintTravel(int signalNumber){
	sigIntTravel = true;
}

/* The following function is used to handle SIGQUIT signal in travelMonitor process. */
void sigquitTravel(int signalNumber){
	sigQuitTravel = true;
}