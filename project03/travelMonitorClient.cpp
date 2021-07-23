#include "travelMonitorClient.hpp"

int main(int argc,char**argv){

	if(argc != expectedArgs){

		perror("Wrong parameter given at travelMonitorClient.");
		return 0;
	}

	char currentPath[maxFileNameSize];
	getcwd(currentPath, sizeof(currentPath));

	int numMonitors;
	int socketBufferSize;
	int cyclicBufferSize;
	int sizeOfBloom;
	char* input_dir;
	int numThreads;

	/* Store argument values. */
	for(int i=0;i<argc;i++)
		if(!strcmp(argv[i],"-m"))
			numMonitors = stoi(argv[++i]);
		else if(!strcmp(argv[i],"-b"))
			socketBufferSize = stoi(argv[++i]);
		else if(!strcmp(argv[i],"-c"))
			cyclicBufferSize = stoi(argv[++i]);
		else if(!strcmp(argv[i],"-s"))
			sizeOfBloom = stoi(argv[++i]);
		else if(!strcmp(argv[i],"-i"))
			input_dir = argv[++i];
		else if(!strcmp(argv[i],"-t"))
			numThreads = stoi(argv[++i]);

	/* We initialize a directories list which hold all directories that take part in our execution. 
	There is one list for each monitor process. */
	DirectoriesList* dl[numMonitors];
	for(int i=0;i<numMonitors;i++)
		dl[i] = new DirectoriesList(); 

	DIR *d;
	struct dirent *dir;
	d = opendir(input_dir);

	int count =0;
	int totalCountries =0;

	if(d){

		/* Get all directories of "input_dir" and concat them on a single string, one for each Monitor. */
		while((dir = readdir(d)) != NULL){
			
			if(strcmp(dir->d_name,".") == 0 || strcmp(dir->d_name,"..") == 0)
				continue;

			/* Insert all directories in the current monitor's list (R-R), by parsing each folder 'input_dir' directory. */
			dl[count]->insert(dir->d_name);
			
			count = (count + 1) % numMonitors;
			totalCountries++;
		}
		closedir(d);
	}

	/* Here we initialize arguments array that will be used in execvp in order to start all monitor processes.
	Because of the fact that parameter's size is variable, we initialize the array with the size of all arguments
	that must be inserted plus the maximum amount of countries that each monitor can get as arguments, which is
	the total amount of countries that exist in 'input_dir. */
	int totalArguments = expectedArgsChild + totalCountries + 1;
	char** vector = (char**)malloc(sizeof(char*)*(totalArguments+1));
	for(int i=0;i<totalArguments;i++)
		vector[i] = (char*)malloc(maxFileNameSize);

	vector[totalArguments] = NULL;

	struct hostent *host;
	char buffer[256];
	char* hostIP;
	int hostName;
	char* IPstring;

    hostName = gethostname(buffer, sizeof(buffer));
    host = gethostbyname(buffer);

    /* Here we get the ip of this device, in order to use it for socket communication. */
    IPstring = inet_ntoa(*((struct in_addr*) host->h_addr_list[0]));

	int pid;
	int childPorts[numMonitors];
	int childSockets[numMonitors];
	
	for(int i=0;i<numMonitors;i++){

		for(int i=0;i<totalArguments;i++)
			strcpy(vector[i]," ");

		childPorts[i] = port + i;

		/* Here we fill the arguments array with the parameters that we want to give to each monitor process. */
		strcpy(vector[0],"./monitorServer");
		strcpy(vector[1],"-p");
		strcpy(vector[2],to_string(childPorts[i]).c_str());
		strcpy(vector[3],"-t");
		strcpy(vector[4],to_string(numThreads).c_str());
		strcpy(vector[5],"-b");
		strcpy(vector[6],to_string(socketBufferSize).c_str());
		strcpy(vector[7],"-c");
		strcpy(vector[8],to_string(cyclicBufferSize).c_str());
		strcpy(vector[9],"-s");
		strcpy(vector[10],to_string(sizeOfBloom).c_str());

		count = 11;

		DirectoriesList* tmpList = dl[i];

		while(tmpList != NULL){

			char tmpPath[maxFileNameSize];
			strcpy(tmpPath,currentPath);
			strcat(tmpPath,"/");
			strcat(tmpPath,input_dir);
			strcat(tmpPath,"/");
			strcat(tmpPath,tmpList->getCountry().c_str());
			strcat(tmpPath,"/");

			strcpy(vector[count++],tmpPath);

			tmpList = tmpList->getNext();

		}

		/* Fork current process in order to create a child process. */
		pid = fork();

		/* For every child process, call execvp in order to create a monitor process. */
		if(pid == 0)
			if(execvp("./monitorServer",vector) < 0){

				perror("Error on creating child process");
				return 0;
			}

		struct sockaddr_in server;

		/* Initialize a socket, one for each monitor process. */
		if((childSockets[i] = socket(AF_INET,SOCK_STREAM,0)) < 0){

			cout << "Error on socket." << endl;
			return 0;
		}

		server.sin_family = AF_INET ; /* Internet domain */
		server.sin_port = htons(childPorts[i]) ; /* Server port */

		if(inet_pton(AF_INET, IPstring, &server.sin_addr)<=0){
			
	        printf("\nInvalid address/ Address not supported \n");
	        return -1;
	    }

	    /* Wait till current socket connects with server, aka corresponding monitor process. */
		while( connect ( childSockets[i] , (struct sockaddr *)&server , sizeof ( server ) ) < 0 ){}

	}

	char *bloomArray,*virusName;

	/* BloomFiltersList holds, for each monitor process, its bloom arrays. */ 
	BloomFiltersList* bflArray[numMonitors];
	for(int i=0;i<numMonitors;i++)
		bflArray[i] = new BloomFiltersList(sizeOfBloom,10);

	/* 'select' initialization. */
	fd_set rfds , rfds2;
   	struct timeval tv;
   	int maxFileDescriptor = -1;

   	FD_ZERO(&rfds2);
   	
   	for(int i=0;i<numMonitors;i++)
   		getMaxFd(childSockets[i],&rfds2,&maxFileDescriptor);

   	tv.tv_sec = 0;
   	tv.tv_usec = 0;

	int gotEnd = 0;

	/* End flag is used in order to indicate that a monitor process has sent all its bloom arrays. */
	bool endFlag[numMonitors];
	memset(endFlag,false,numMonitors);
	
	/* The following loop reads all bloomArrays from monitor processes. */
	while(1){

		rfds = rfds2;
		select(maxFileDescriptor+1, &rfds, NULL, NULL, &tv);

		/* We use select in order to read only from file descriptors that have data to be read.
		This is used to make socket communication non blocking. */
		for(int i=0;i<numMonitors;i++)	
			if(FD_ISSET(childSockets[i], &rfds)){

				/* If monitor process has already sent all its bloom arrays, continue with the next one. */
				if(endFlag[i] == true) continue;
				
				bloomArray = receiveMessage(childSockets[i],socketBufferSize,'^');
				
				/* If end message was received, it means that a monitor is done sending arrays.
				We will stop receiving messages when all monitors has sent an "end" messsage. */
				if( !strcmp(bloomArray,"end") ){

					endFlag[i] = true;
					gotEnd++;
					free(bloomArray);	
					continue;
				}
								
				virusName = receiveMessage(childSockets[i],socketBufferSize,'^');

				/* Insert bloom array received, in BloomFiltersList. */
				bflArray[i]->insert(virusName,bloomArray);

				free(bloomArray);
				free(virusName);
			}

		if(gotEnd == numMonitors) break;
	}

	/* Separator string is used in order to indicate the end of a message inside the buffer. More on that in 'messageProtocol.cpp'. */
	char *sep = (char*)malloc(strlen("^")+1);
	strcpy(sep,"^");

	int totalRequests =0;
	int totalAccepted =0;
	int totalRejected =0;
	string input ,function;
	RequestList rl;

	/* This loop is the main functionality of the program. */
	while(1){

		/* Here we start reading input from user. For every function that user inserts, we get the data we need
		from inputStream. */
		cout << endl << "Give me your action: ";

		getline(cin,input);

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
			sendMessage(childSockets[tempMonitor],strlen(tmp),tmp,socketBufferSize,sep);
			sendMessage(childSockets[tempMonitor],strlen(tmpCitizenID),tmpCitizenID,socketBufferSize,sep);
			sendMessage(childSockets[tempMonitor],strlen(tmpVirus),tmpVirus,socketBufferSize,sep);

			char* answer = receiveMessage(childSockets[tempMonitor],socketBufferSize,'^');

			if( !strcmp(answer,"NO") ){

				/* Every time, either if request was accepted or not, we insert request to a 
				RequestList for later on. */
				rl.insert(date,countryTo,virusName,false);
				cout << "REQUEST REJECTED - YOU ARE NOT VACCINATED" << endl;
				totalRejected++;
			
			}else{

				char* tmpDate= receiveMessage(childSockets[tempMonitor],socketBufferSize,'^');
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

			/* First find which monitor holds data for country given as input. */
			int tempMonitor = getMonitorByDir(dl,numMonitors,country);

			char tmp[9999];
			strcpy(tmp,"addVaccinationRecords");
				
			/* Send addVaccinationRecords flag in specific monitor process in order for him to start the process. */	
			sendMessage(childSockets[tempMonitor],strlen(tmp),tmp,socketBufferSize,sep);

			char tmpCountryPath[9999];
			strcpy(tmpCountryPath,currentPath);
			strcat(tmpCountryPath,"/");
			strcat(tmpCountryPath,input_dir);
			strcat(tmpCountryPath,"/");
			strcat(tmpCountryPath,country.c_str());

			/* We send the path where the new files are to the corresponding monitor. */
			sendMessage(childSockets[tempMonitor],strlen(tmpCountryPath),tmpCountryPath,socketBufferSize,sep);

			delete bflArray[tempMonitor];
			bflArray[tempMonitor] = new BloomFiltersList(sizeOfBloom,10);

			memset(endFlag,false,numMonitors);

			/* And then receive the new bloomArrays. */
			while(1){
				
				bloomArray = receiveMessage(childSockets[tempMonitor],socketBufferSize,'^');

				/* Again read arrays till we receive end message. */
				if( !strcmp(bloomArray,"end") ){

					free(bloomArray);
					break;
				}
				
				virusName = receiveMessage(childSockets[tempMonitor],socketBufferSize,'^');

				bflArray[tempMonitor]->insert(virusName,bloomArray);
				free(virusName);
				free(bloomArray);

			}

		}else if(function == "/searchVaccinationStatus"){

			string citizenID ;
			inputStream >> citizenID;

			char tmpCitizenID[9999], tmp[9999];
			strcpy(tmpCitizenID,citizenID.c_str());
			strcpy(tmp,"searchVaccinationStatus");

			/* Send messages to each monitor process to indicate which function they need to execute and also
			give them the id of the citizen they must search. */
			for(int i=0;i<numMonitors;i++){

				sendMessage(childSockets[i],strlen(tmp),tmp,socketBufferSize,sep);
				sendMessage(childSockets[i],strlen(tmpCitizenID),tmpCitizenID,socketBufferSize,sep);
			}

			/* 'Select' initialization. */
			maxFileDescriptor = -1;
			gotEnd = 0;
			memset(endFlag,false,numMonitors);
			FD_ZERO(&rfds2);
   	
		   	for(int i=0;i<numMonitors;i++)
		   		getMaxFd(childSockets[i],&rfds2,&maxFileDescriptor);

		   	tv.tv_sec = 0;
		   	tv.tv_usec = 0;
			
			/* We also use select here in order to receive data each time only from a fifo that has data to read. 
			Here we read bloomArrays again, because data has changed so we need to update our BloomFiltersList. */
			while(1){

				rfds = rfds2;
				select(maxFileDescriptor+1, &rfds, NULL, NULL, &tv);

				for(int i=0;i<numMonitors;i++)	
					if(FD_ISSET(childSockets[i], &rfds)){

						if(endFlag[i] == true) continue;

						char *message = receiveMessage(childSockets[i],socketBufferSize,'^');
						
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
		/* If exit was given by the user, first we create a log file and write data to it, then we send 'exit' 
		message to all monitor processes, in order for them to begin exit process too. */
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

			logfile.close();

			string citizenID ;
			inputStream >> citizenID;

			char tmp[9999];
			strcpy(tmp,"exit");

			/* Send messages to each monitor process to indicate which function they need to execute and also
			give them the id of the citizen they must search. */
			for(int i=0;i<numMonitors;i++)
				sendMessage(childSockets[i],strlen(tmp),tmp,socketBufferSize,sep);

			for(int i=0;i<numMonitors;i++)
				close(childSockets[i]);

			break;

		}else
			cout << "ERROR.WRONG INPUT" << endl;

	}

	for(int i=0;i<numMonitors;i++){

		delete dl[i];
		delete bflArray[i];
	}

	for(int i=0;i<totalArguments;i++)
		free(vector[i]);

	free(vector);
	free(sep);
}