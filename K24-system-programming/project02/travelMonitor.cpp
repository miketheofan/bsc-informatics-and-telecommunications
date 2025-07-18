#include "travelMonitor.hpp"

/* Declaring signal flags. */
bool addVaccinationRecords;
int childKilled;
bool sigIntMonitor;
bool sigQuitMonitor;
bool sigIntTravel;
bool sigQuitTravel;

int main(int argc , char** argv){ 

	sigIntMonitor = false;
	sigQuitMonitor = false;

	sigIntTravel = false;
	sigQuitTravel = false;

	/* Initializing SIGINT signal. */
	struct sigaction intSignal;
	memset(&intSignal,0,sizeof(intSignal));

	intSignal.sa_handler = &sigintTravel;
	intSignal.sa_flags = SA_RESTART | SA_SIGINFO;	

	if(sigaction(SIGINT, &intSignal, NULL) < 0){

		cout << "Error on sigint.Exiting child process..." << endl;
		return 0;
	}

	/* Initializing SIGQUIT signal. */
	struct sigaction quitSignal;
	memset(&quitSignal,0,sizeof(quitSignal));

	quitSignal.sa_handler = &sigquitTravel;
	quitSignal.sa_flags = SA_RESTART | SA_SIGINFO;	

	if(sigaction(SIGQUIT, &quitSignal, NULL) < 0){

		cout << "Error on sigquit.Exiting child process..." << endl;
		return 0;
	}

	addVaccinationRecords = false;
	childKilled = -1;

	signal(SIGCHLD,&sigChild);

	int numMonitors, bufferSize;
	int sizeOfBloom;
	char *input_dir;

	int totalRequests = 0;
	int totalAccepted = 0;
	int totalRejected = 0;
	
	char currentPath[maxFileNameSize];
	getcwd(currentPath, sizeof(currentPath));

	if(argc != expectedArgsParent){

		cout << "Wrong input.Exiting parent process..." << endl;
		return 0;
	}

	for(int i=0;i<argc;i++)
		if(!strcmp(argv[i],"-m"))
			numMonitors = stoi(argv[++i]);
		else if(!strcmp(argv[i],"-b"))
			bufferSize = stoi(argv[++i]);
		else if(!strcmp(argv[i],"-s"))
			sizeOfBloom = stol(argv[++i]);
		else if(!strcmp(argv[i],"-i"))
			input_dir = argv[++i];

	string fifoPath[numMonitors][2];

	char** vector = (char**)malloc(sizeof(char*)*(expectedArgsChild+1));
	for(int i=0;i<expectedArgsChild;i++)
		vector[i] = (char*)malloc(maxFileNameSize);

	vector[expectedArgsChild] = NULL;
	
	int pid, childPids[numMonitors];

	/* For every monitor, we intialize an array with his parameters, create his fifo's, fork a process and 
	use execvp in order to run his executable. */
	for(int i=0;i<numMonitors;i++){
		
		fifoPath[i][0] += currentPath;
		fifoPath[i][0] += "/pipes/parentPipe" + to_string(i);
		fifoPath[i][1] += currentPath;
		fifoPath[i][1] += "/pipes/childPipe" + to_string(i);

		unlink(fifoPath[i][0].c_str());
		unlink(fifoPath[i][1].c_str());

		if( mkfifo(fifoPath[i][0].c_str(), 0700) < 0 ){

			cout << "Error on creating writting named pipe " << i << endl;
			return 0;
		}

		if( mkfifo(fifoPath[i][1].c_str(), 0700) < 0 ){

			cout << "Error on creating writting named pipe " << i << endl;
			return 0;
		}

		pid = fork();
		childPids[i] = pid;

		strcpy(vector[0],"./Monitor");
		strcpy(vector[1],"-m1");
		strcpy(vector[2],fifoPath[i][0].c_str());
		strcpy(vector[3],"-m2");
		strcpy(vector[4],fifoPath[i][1].c_str());

		if(pid == 0)
			if(execvp("./Monitor",vector) < 0){

				cout << "Error on creating child process number " << i << endl;
				return 0;
			}
	}

	DIR *d;
	struct dirent *dir;
	d = opendir(input_dir);

	DirectoriesList* dl[numMonitors];
	for(int i=0;i<numMonitors;i++)
		dl[i] = new DirectoriesList(); 

	int count = 0;
	
	char directory[numMonitors][9999];
	for(int i=0;i<numMonitors;i++){

		strcpy(directory[i],input_dir);
		strcat(directory[i],"/");
	}

	if(d){

		/* Get all directories of "input_dir" and concat them on a single string, one for each Monitor. */
		while((dir = readdir(d)) != NULL){
			
			if(strcmp(dir->d_name,".") == 0 || strcmp(dir->d_name,"..") == 0)
				continue;

			strcat(directory[count],dir->d_name);
			strcat(directory[count],"/");

			dl[count]->insert(dir->d_name);
			
			count = (count + 1) % numMonitors;
		}
		closedir(d);
	}

	int fd[numMonitors][2];

	char bloomString[bufferSize];
	snprintf(bloomString,10,"%d",sizeOfBloom);
	
	/* sep is a string used in all messages that will be sent from parent process, and is used in order
	to indicate the end of a message. */
	char *sep = (char*)malloc(strlen("^")+1);
	strcpy(sep,"^");

	/* Here we initialize each monitor by sending him the bufferSize, the sizeOfBloom and the directories
	which he will use to fill his data structures. */
	for(int i=0;i<numMonitors;i++){

		fd[i][0] = open(fifoPath[i][0].c_str(), O_WRONLY);
		fd[i][1] = open(fifoPath[i][1].c_str(), O_RDONLY);

		sendFirstMessage(fd[i][0], bufferSize);
		sendMessage(fd[i][0],sizeOfBloom,bloomString,bufferSize,sep);
		sendMessage(fd[i][0],strlen(directory[i]),directory[i],bufferSize,sep);

	}

	char *bloomArray,*virusName;

	BloomFiltersList* bflArray[numMonitors];
	for(int i=0;i<numMonitors;i++)
		bflArray[i] = new BloomFiltersList(sizeOfBloom,10);

	fd_set rfds , rfds2;
   	struct timeval tv;
   	int maxFileDescriptor = -1;

   	FD_ZERO(&rfds2);
   	
   	for(int i=0;i<numMonitors;i++)
   		getMaxFd(fd[i][1],&rfds2,&maxFileDescriptor);

   	tv.tv_sec = 0;
   	tv.tv_usec = 0;

	int gotEnd = 0;
	bool endFlag[numMonitors];
	memset(endFlag,false,numMonitors);
	
	/* The following loop reads all bloomArrays from monitor processes. */
	while(1){

		rfds = rfds2;
		select(maxFileDescriptor+1, &rfds, NULL, NULL, &tv);

		/* We use select in order to read only from file descriptors that have data to be read.
		This is used to make fifo's non blocking. */
		for(int i=0;i<numMonitors;i++)	
			if(FD_ISSET(fd[i][1], &rfds)){

				if(endFlag[i] == true) continue;
				
				bloomArray = receiveMessage(fd[i][1],bufferSize,'^');
				/* If end message was received, it means that a monitor is done sending arrays.
				We will stop receiving messages when all monitors has sent an "end" messsage. */
				if( !strcmp(bloomArray,"end") ){

					endFlag[i] = true;
					gotEnd++;
					free(bloomArray);	
					continue;
				}
				
				virusName = receiveMessage(fd[i][1],bufferSize,'^');

				bflArray[i]->insert(virusName,bloomArray);

				free(bloomArray);
				free(virusName);
			}

		if(gotEnd == numMonitors) break;
	}

	string input ,function;
	RequestList rl;

	/* This loop is the main functionality of the program. */
	while(1){

		/* First of all check if SIGCHILD flag was raised. If flag is not -1 it will hold
		a child's pid. */
		if(childKilled != -1){

			int i;
			for(i=0;i<numMonitors;i++)
				if(childPids[i] == childKilled)
					break;

			/* First we delete his bloomArrays, and then we initialize a new monitor that will
			replace the killed one. */
			delete bflArray[i];
			close(fd[i][0]);
			close(fd[i][1]);

			pid = fork();
			childPids[i] = pid;

			strcpy(vector[0],"./Monitor");
			strcpy(vector[1],"-m1");
			strcpy(vector[2],fifoPath[i][0].c_str());
			strcpy(vector[3],"-m2");
			strcpy(vector[4],fifoPath[i][1].c_str());

			if(pid == 0)
				if(execvp("./Monitor",vector) < 0){

					cout << "Error on creating child process number " << i << endl;
					return 0;
				}

			fd[i][0] = open(fifoPath[i][0].c_str(), O_WRONLY);
			fd[i][1] = open(fifoPath[i][1].c_str(), O_RDONLY);

			sendFirstMessage(fd[i][0], bufferSize);
			sendMessage(fd[i][0],sizeOfBloom,bloomString,bufferSize,sep);
			sendMessage(fd[i][0],strlen(directory[i]),directory[i],bufferSize,sep);
		
			/* Then we read all its bloomArrays again. */
			bflArray[i] = new BloomFiltersList(sizeOfBloom,10);
			while(1){
				
				bloomArray = receiveMessage(fd[i][1],bufferSize,'^');
				if( !strcmp(bloomArray,"end") )
					break;
				
				virusName = receiveMessage(fd[i][1],bufferSize,'^');

				bflArray[i]->insert(virusName,bloomArray);

			}

			childKilled = -1;
		}

		/* If SIGQUIT or SIGINT flags were raised, we create a log file and write all the information needed. */
		if(sigIntTravel == true || sigQuitTravel == true){

			ofstream logfile;

			string logfileName;
			logfileName.append("log_file.");
			logfileName.append(to_string(getpid()));

			logfile.open(logfileName);

			for(int i=0;i<numMonitors;i++){

				DirectoriesList* tmpList = dl[i];
				
				while(tmpList != NULL){

					logfile << tmpList->getCountry();
					logfile << "\n";
					tmpList = tmpList->getNext();
				}
			}

			logfile << "TOTAL TRAVEL REQUESTS " << totalRequests;
			logfile << "\nACCEPTED " << totalAccepted;
			logfile << "\nREJECTED " << totalRejected;

			if(sigIntMonitor == true) sigIntMonitor = false;
			if(sigQuitMonitor == true) sigQuitMonitor = false;

			/* Then we kill all child processes. */
			for(int i=0;i<numMonitors;i++){

				int status = 0;
				kill(childPids[i],status);
			}

			/* And return. */
			return 0;
		}

		/* Here we start reading input from user. For every function that user inserts, we get the data we need
		from inputStream. */
		cout << endl << "Give me your action: ";

		getline(cin,input);

		if(input.empty()) continue;

		if(input == "") continue;
		istringstream inputStream(input);
		inputStream >> function;

		if(function == "/travelRequest"){

			totalRequests++;

			string citizenID ,date , countryFrom , countryTo , virusName;
			inputStream >> citizenID >> date >> countryFrom >> countryTo >> virusName;

			/* First we must know which monitor process we need to get data from. */
			int tempMonitor = getMonitorByDir(dl,numMonitors,countryFrom);
			
			/* If citizen is not vaccinated in the bloomArray of parent process, we reject him. */
			if(!bflArray[tempMonitor]->isVaccinated(stoi(citizenID),virusName) == true){

				cout << "REQUEST REJECTED - YOU ARE NOT VACCINATED" << endl;
				totalRejected++;
				rl.insert(date,countryTo,virusName,false);
				continue;
			}

			char tmp[9999], tmpCitizenID[9999], tmpVirus[9999];
			strcpy(tmp,"travelRequest");
			strcpy(tmpCitizenID,citizenID.c_str());
			strcpy(tmpVirus,virusName.c_str());

			/* Otherwise we must ask for data from a monitor process. */
			sendMessage(fd[tempMonitor][0],strlen(tmp),tmp,bufferSize,sep);
			sendMessage(fd[tempMonitor][0],strlen(tmpCitizenID),tmpCitizenID,bufferSize,sep);
			sendMessage(fd[tempMonitor][0],strlen(tmpVirus),tmpVirus,bufferSize,sep);

			char* answer = receiveMessage(fd[tempMonitor][1],bufferSize,'^');

			if( !strcmp(answer,"NO") ){

				/* Every time, either if request was accepted or not, we insert request to a 
				RequestList for later on. */
				rl.insert(date,countryTo,virusName,false);
				cout << "REQUEST REJECTED - YOU ARE NOT VACCINATED" << endl;
				totalRejected++;
			
			}else{

				char* tmpDate= receiveMessage(fd[tempMonitor][1],bufferSize,'^');
				string dateVaccinated = tmpDate;
				if(dateOK(dateVaccinated,date)){

					rl.insert(date,countryTo,virusName,true);
					cout << "REQUEST ACCEPTED - HAPPY TRAVELS" << endl;
					totalAccepted++;

				}else{

					rl.insert(date,countryTo,virusName,false);
					cout << "REQUEST REJECTED = YOU WILL NEED ANOTHER VACCINATION BEFORE TRAVEL DATE" << endl;
					totalRejected++;

				}

				free(tmpDate);
			}

			free(answer);

		}else if(function == "/travelStats"){

			string country , virusName , date1 , date2;
			inputStream >> country >> virusName >> date1 >> date2;

			/* In this function we use an array of results, which will be filled with data from RequestList
			that was filled previously and then we print the data that we got. */
			int* results = new int[3];
			for(int i=0;i<3;i++)
				results[i] = 0;

			if(!rl.isEmpty()){
				if(date2.empty())
					rl.travelStats(&results,country,virusName,date1);
				else						
					rl.travelStats(&results,country,virusName,date1,date2);
			}

			cout << "TOTAL REQUESTS " << results[0] << endl;
			cout << "ACCEPTED " << results[1] << endl;
			cout << "REJECTED " << results[2] << endl;

			delete[] results;

		}else if(function == "/addVaccinationRecords"){

			string country;
			inputStream >> country;

			int tempMonitor = getMonitorByDir(dl,numMonitors,country);

			kill(childPids[tempMonitor],SIGUSR1);

			char tmpCountryPath[9999];
			strcpy(tmpCountryPath,input_dir);
			strcat(tmpCountryPath,"/");
			strcat(tmpCountryPath,country.c_str());

			/* We send the path where the new files are to the corresponding monitor. */
			sendMessage(fd[tempMonitor][0],strlen(tmpCountryPath),tmpCountryPath,bufferSize,sep);

			delete bflArray[tempMonitor];
			bflArray[tempMonitor] = new BloomFiltersList(sizeOfBloom,10);

			memset(endFlag,false,numMonitors);

			/* And then receive the new bloomArrays. */
			while(1){
				
				bloomArray = receiveMessage(fd[tempMonitor][1],bufferSize,'^');
				if( !strcmp(bloomArray,"end") ){

					free(bloomArray);
					break;
				}
				
				virusName = receiveMessage(fd[tempMonitor][1],bufferSize,'^');

				bflArray[tempMonitor]->insert(virusName,bloomArray);

			}

			addVaccinationRecords = false;

		}else if(function == "/searchVaccinationStatus"){

			string citizenID ;
			inputStream >> citizenID;

			char tmpCitizenID[9999], tmp[9999];
			strcpy(tmpCitizenID,citizenID.c_str());
			strcpy(tmp,"searchVaccinationStatus");

			/* Send messages to each monitor process to indicate which function they need to execute and also
			give them the id of the citizen they must search. */
			for(int i=0;i<numMonitors;i++){

				sendMessage(fd[i][0],strlen(tmp),tmp,bufferSize,sep);
				sendMessage(fd[i][0],strlen(tmpCitizenID),tmpCitizenID,bufferSize,sep);
			}

			maxFileDescriptor = -1;
			gotEnd = 0;
			memset(endFlag,false,numMonitors);
			FD_ZERO(&rfds2);
   	
		   	for(int i=0;i<numMonitors;i++)
		   		getMaxFd(fd[i][1],&rfds2,&maxFileDescriptor);

		   	tv.tv_sec = 0;
		   	tv.tv_usec = 0;
			
			/* We also use select here in order to receive data each time only from a fifo that has data to read. */
			while(1){

				rfds = rfds2;
				select(maxFileDescriptor+1, &rfds, NULL, NULL, &tv);

				for(int i=0;i<numMonitors;i++)	
					if(FD_ISSET(fd[i][1], &rfds)){

						if(endFlag[i] == true) continue;

						char *message = receiveMessage(fd[i][1],bufferSize,'^');
						
						if( !strcmp(message,"end") ) {

							endFlag[i] = true;
							gotEnd++;	
							free(message);
							continue;
						}

						cout << message << endl;

						free(message);
					}

				if(gotEnd == numMonitors) break;

			}
		/* If exit was given by the user, first we create a log file and write data to it, then we kill all 
		monitor processes, wait for them to finish and return. */
		}else if(function == "/exit"){

			ofstream logfile;

			string logfileName;
			logfileName.append("log_file.");
			logfileName.append(to_string(getpid()));

			logfile.open(logfileName);

			for(int i=0;i<numMonitors;i++){

				DirectoriesList* tmpList = dl[i];
				
				while(tmpList != NULL){

					logfile << tmpList->getCountry();
					logfile << "\n";
					tmpList = tmpList->getNext();
				}
			}

			logfile << "TOTAL TRAVEL REQUESTS " << totalRequests;
			logfile << "\nACCEPTED " << totalAccepted;
			logfile << "\nREJECTED " << totalRejected;

			for(int i=0;i<numMonitors;i++){

				int status = 0;
				kill(childPids[i],status);
				waitpid(childPids[i],&status,WNOHANG);
			}

			logfile.close();

			break;

		}else
			cout << "ERROR.WRONG INPUT" << endl;

	}

	for(int i=0;i<expectedArgsChild;i++)
		free(vector[i]);

	free(sep);
	free(vector);

	for(int i=0;i<numMonitors;i++){

		delete bflArray[i];
		delete dl[i];

		close(fd[i][0]);
		close(fd[i][1]);
	}

	return 0;
}



