#include "Monitor.hpp"

/* Declaring signal flags. */
bool addVaccinationRecords;
int childKilled;
bool sigIntMonitor;
bool sigQuitMonitor;
bool sigIntTravel;
bool sigQuitTravel;

int main(int argc , char** argv){ 

	/* Initializing SIGUSR1 signal. */
	struct sigaction functionSignal;
	memset(&functionSignal,0,sizeof(functionSignal));

	functionSignal.sa_sigaction = &mySignal;
	functionSignal.sa_flags = SA_RESTART | SA_SIGINFO;

	if(sigaction(SIGUSR1, &functionSignal, NULL) != 0){

		cout << "Error on sigaction.Exiting child process..." << endl;
		return 0;
	}

	/* Initializing SIGCHILD signal. */
	struct sigaction childSignal;
	memset(&childSignal,0,sizeof(childSignal));

	childSignal.sa_handler = &sigChild;
	childSignal.sa_flags = SA_RESTART | SA_SIGINFO;	

	if(sigaction(SIGCHLD, &childSignal, NULL) < 0){

		cout << "Error on sigchild.Exiting child process..." << endl;
		return 0;
	}

	/* Initializing SIGINT signal. */
	struct sigaction intSignal;
	memset(&intSignal,0,sizeof(intSignal));

	intSignal.sa_handler = &sigintMonitor;
	intSignal.sa_flags = SA_RESTART | SA_SIGINFO;	

	if(sigaction(SIGINT, &intSignal, NULL) < 0){

		cout << "Error on sigint.Exiting child process..." << endl;
		return 0;
	}

	/* Initializing SIGQUIT signal. */
	struct sigaction quitSignal;
	memset(&quitSignal,0,sizeof(quitSignal));

	quitSignal.sa_handler = &sigquitMonitor;
	quitSignal.sa_flags = SA_RESTART | SA_SIGINFO;	

	if(sigaction(SIGQUIT, &quitSignal, NULL) < 0){

		cout << "Error on sigquit.Exiting child process..." << endl;
		return 0;
	}

	string readPipe,writePipe;
	int sizeOfBloom = 0;
	int bufferSize;

	int totalRequests = 0;
	int totalAccepted = 0;
	int totalRejected = 0;

	/* Check if arguments were given as exprected. */
	if(argc != expectedArgsChild){

		cout << "Wrong input.Exiting child process..." << endl;
		return 0;
	}

	/* Store argument values. */
	for(int i=0;i<argc;i++)
		if(!strcmp(argv[i],"-m1"))
			readPipe = argv[++i];
		else if(!strcmp(argv[i],"-m2"))
			writePipe = argv[++i];

	/* Open both named pipes, one for reading from parent and one for writting to parent. */
	int fdRead = open(readPipe.c_str(), O_RDONLY, 0);
	int fdWrite = open(writePipe.c_str(), O_WRONLY, 0);
	
	DirectoriesList *directories;
	BloomList *blooms;

	/* Call function that initializes a monitor. By initializing, we mean reading everything we need from
	parent thourgh the corresponding fifo, and filling all data structures we will use for this program. */
	initializeMonitor(fdRead,&bufferSize,&sizeOfBloom,&blooms,&directories);

	BloomList* tempList = blooms;

	/* Here we send all bloomFilters that were filled during the initialization to parent. */
	while(tempList != NULL){

		sendBloom(fdWrite,sizeOfBloom,tempList->getCells(),bufferSize,tempList->getVirus());
		tempList = tempList->getNext();
	}

	/* sep string is used in order to indicate the end of a message. It is used as an argument to sendMessage function. */
	char *sep = (char*)malloc(strlen("^")+1);
	strcpy(sep,"^");

	/* When all bloomFilters are sent, we send "end", in order for parent process to realize that monitor is done sending
	bloomFilters. */
	char endMessage[bufferSize];
	strcpy(endMessage,"end");

	sendMessage(fdWrite,strlen(endMessage),endMessage,bufferSize,sep);

	char* tmp;

	/* The following while loop is used to manage the main functionality of monitors. */
	while(1){

		fd_set rfds , rfds2;
	   	struct timeval tv;
	   	int maxFileDescriptor = -1;

	   	FD_ZERO(&rfds2);
	   	
	   	getMaxFd(fdRead,&rfds2,&maxFileDescriptor);

	   	tv.tv_sec = 0;
	   	tv.tv_usec = 0;

		rfds = rfds2;
		select(maxFileDescriptor+1, &rfds, NULL, NULL, &tv);

		/* If SIGINT or SIGQUIT was raised for this particular monitor then
		we create the logfile continue with the loop. */
		if(sigIntMonitor == true || sigQuitMonitor == true){

			ofstream logfile;

			string logfileName;
			logfileName.append("log_file.");
			logfileName.append(to_string(getpid()));

			logfile.open(logfileName);

			DirectoriesList* tmpList = directories;
			
			while(tmpList != NULL){

				logfile << tmpList->getCountry();
				logfile << "\n";
				tmpList = tmpList->getNext();
			}

			logfile << "TOTAL TRAVEL REQUESTS " << totalRequests;
			logfile << "\nACCEPTED " << totalAccepted;
			logfile << "\nREJECTED " << totalRejected;

			/* Here we reset the signal flags. */
			if(sigIntMonitor == true) sigIntMonitor = false;
			if(sigQuitMonitor == true) sigQuitMonitor = false;

			logfile.close();
		}

		/* If there is data to read in fifo */
		if(FD_ISSET(fdRead, &rfds)){

			tmp = receiveMessage(fdRead,bufferSize,'^');
		
			/* If receive message returned NULL, it means that this monitor was waiting for data from fifo
			even though parent process has terminated. In this case we break and wait for parent process
			to kill curent monitor. */
			if(tmp == NULL) break;

			/* We check if for first message key string : "travelRequest" was sent. In this case
			we must function as described. */
			if( !strcmp(tmp,"travelRequest") ){

				totalRequests++;

				/* First we get the data we need from fifo. */
				char *citizenID = receiveMessage(fdRead,bufferSize,'^');
				char *virusName = receiveMessage(fdRead,bufferSize,'^');

				char answer[9999];
				string date;

				/* If citizen exists in skip lists then send "YES" and the date he was vaccinated. */
				if( (date = blooms->isVaccinated(stoi(citizenID),virusName)) != " " ){

					strcpy(answer,"YES");
					sendMessage(fdWrite,strlen(answer),answer,bufferSize,sep);
					char tmpDate[9999];
					strcpy(tmpDate,date.c_str());
					sendMessage(fdWrite,strlen(tmpDate),tmpDate,bufferSize,sep);
				
					totalAccepted++;

				}else{

					/* Otherwise send "NO". */
					strcpy(answer,"NO");
					sendMessage(fdWrite,strlen(answer),answer,bufferSize,sep);
			
					totalRejected++;	

				}

			/* We check if for first message key string : "searchVaccinationStatus" was sent. In this case
			we must function as described. */
			}else if( !strcmp(tmp,"searchVaccinationStatus") ){

				/* We first read the data we need from fifo. */
				char* tmpCitizenID = receiveMessage(fdRead,bufferSize,'^');
				int id = stoi(tmpCitizenID);

				string fm = blooms->getCitizen(id);
				string sm = blooms->getAge(id);

				/* Then we search in our BloomList if entries for this particular id exists and insert them in our RecordsList. */
				RecordsList* rl = new RecordsList;
				blooms->searchVaccinationStatus(&rl,id);

				char tmpfm[9999] , tmpsm[9999];

				/* If data were found for this id, then send them to the parent process. */
				if(fm != " "){

					/* First we send citizen's data. */
					strcpy(tmpfm,fm.c_str());
					sendMessage(fdWrite,strlen(tmpfm),tmpfm,bufferSize,sep);

					/* Then we send his age. */
					strcpy(tmpsm,sm.c_str());
					sendMessage(fdWrite,strlen(tmpsm),tmpsm,bufferSize,sep);

					char send[9999];

					/* And finally all his records for all viruses that this monitor has data for. */
					while(rl != NULL){

						strcpy(send,rl->getContent().c_str());
						sendMessage(fdWrite,strlen(send),send,bufferSize,sep);

						rl = rl->getNext();
					}
				}

				/* Also send "end" message in order to indicate that we finished sending data. */
				char endMessage[bufferSize];
				strcpy(endMessage,"end");

				sendMessage(fdWrite,strlen(endMessage),endMessage,bufferSize,sep);
				
			}else{

				/* Otherwise check if SIGUSR1 signal was raised. */
				if(addVaccinationRecords == true){

					/* Call function in order to fill data BloomList with the new data that were inserted in directory
					that was sent by parent process. */
					fillNewDirectory(tmp,blooms);

					tempList = blooms;

					/* And send all bloomFilters again in parent process. */
					while(tempList != NULL){

						sendBloom(fdWrite,sizeOfBloom,tempList->getCells(),bufferSize,tempList->getVirus());
						tempList = tempList->getNext();
					}
				
					char endMessage[bufferSize];
					strcpy(endMessage,"end");

					sendMessage(fdWrite,strlen(endMessage),endMessage,bufferSize,sep);

				}
			}

			free(tmp);
		}
	}

	/* Finally we close both fifos. */
	close(fdRead);
	close(fdWrite);
}