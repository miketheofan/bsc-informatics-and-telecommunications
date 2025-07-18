#include "monitorServer.hpp"

/* The following function initializes the cyclicBuffer. */
void initialize(pool_t * pool,int size) {

	pool->data = new string[size];
	pool->POOL_SIZE = size;
	pool->start = 0;
	pool->end = -1;
	pool->count = 0;
}

/* The following function places a new file's path to cyclicBuffer. */
void place(pool_t * pool, string file) {

	/* First we lock mutex in order to make sure current thread can place a new file's path. */
	pthread_mutex_lock(&mtx);

	/* Make sure cyclicBuffer size is not full. */
	while (pool->count >= pool->POOL_SIZE)
		pthread_cond_wait(&cond_nonfull, &mtx);

	pool->end = (pool->end + 1) % pool->POOL_SIZE;
	pool->data[pool->end] = file;
	pool->count++;

	pthread_mutex_unlock(&mtx);

}

/* The following function obtains a file's path from the cyclicBuffer. */
string obtain(pool_t * pool) {

	string data = "";

	/* First make sure current thread is the only one trying to obtain a file's path. */
	pthread_mutex_lock(&mtx);

	/* If cyclicBuffer is empty, wait till a file's path is placed. */
	while (pool->count <= 0)
		pthread_cond_wait(&cond_nonempty, &mtx);

	data = pool->data[pool->start];
	pool->start = (pool->start + 1) % pool->POOL_SIZE;
	pool->count--;

	pthread_mutex_unlock(&mtx);

	return data;

}

int limit = 0;

/* The following function is my implementation for monitor processes' threads. */
void* MonitorThreads(void* arg){

	/* Begin an infinite loop. */
	while(1){

		pthread_mutex_unlock(&mutexAvailable);

		/* If thread has been "woken up", but flag is true then it means that thread must terminate. */
		if(!(totalFiles > 0) && !(pool.count > 0) && terminateThreadsFlag)
			return NULL;

		/* While there are files to read from cyclic buffer. */
		while (totalFiles > 0 || pool.count > 0) {

			/* Get a file's path. */
			string file = obtain(&pool);

			/* Lock mutex that makes sure that only one thread is updating the BloomList. */
			pthread_mutex_lock(&lockBloomList);

			/* Update BloomList. */
			fillNewFile(file,blooms,citizens);

			/* Global Variable limit is used to signal main thread how many files are inserted in the process
			of updating BloomList. */
			limit++;		

			/* Unlock mutex in order for other threads to be able to start their process of updating
			the BloomList. */
			pthread_mutex_unlock(&lockBloomList);

			pthread_cond_signal(&cond_nonfull);
		
		}

		pthread_mutex_lock(&mutexAvailable);

	    pthread_cond_signal(&condAvailable);

	}
}

int main(int argc,char**argv){

	int port;
	int socketBufferSize;
	int cyclicBufferSize;
	int sizeOfBloom;

	/* Total files is a variable that holds at any moment how many files are in the cyclic buffer to be
	read. */
	totalFiles = 0;

	/* Number of Files varaible hold the total amount of files at any moment we need to know that information. */ 
	noFiles  = 0;

	/* DirectoriesList holds all directories that current monitor process has data for. */
	DirectoriesList *list = new DirectoriesList;

	/* Queue is used to "fill" cyclic buffer with file paths. */
	queue = new DirectoriesQueue(totalCountriesWorldWide);

	/* Citizens is a hash table used to hold all citizen records that current monitor process contains,
	in order to free their memory effectively before exiting. The size of the hash table in my implementation. */
	citizens = new citizensList(totalHumansWorldWide);

	/* Store argument values. */
	for(int i=1;i<argc;i++)
		if(!strcmp(argv[i],"-p"))
			port = stoi(argv[++i]);
		else if(!strcmp(argv[i],"-t"))
			numThreads = stoi(argv[++i]);
		else if(!strcmp(argv[i],"-b"))
			socketBufferSize = stoi(argv[++i]);
		else if(!strcmp(argv[i],"-c"))
			cyclicBufferSize = stoi(argv[++i]);
		else if(!strcmp(argv[i],"-s"))
			sizeOfBloom = stoi(argv[++i]);
		else if(!strcmp(argv[i]," "))
			break;
		else{

			string country = argv[i];
			country = country.erase(country.length()-1);
			country = country.substr(country.find_last_of('/')+1,country.length());

			list->insert(country);

			DIR *d;
			struct dirent *dir;
			
			d = opendir(argv[i]);
			while( (dir = readdir(d)) != NULL ){

				if( !strcmp(dir->d_name,".") || !strcmp(dir->d_name,"..")) continue;
				
				string filePath = argv[i];
				filePath += dir->d_name;

				queue->insert(filePath);
				totalFiles++;
				noFiles++;

			}
			closedir(d);
		}	

	/* We initialize BloomList of current monitor process which will hold for country: 2 SkipLists and
	a BloomFilter. */
	blooms = new BloomList("0",sizeOfBloom,noFunctionsFunc,maximumLevelFunc,probabilityFunc);

    int sock,newsocket;
	struct sockaddr_in server,client;
	socklen_t clientlen = sizeof(client) ;
	struct sockaddr *serverptr = (struct sockaddr*)&server;
	struct sockaddr *clientptr = (struct sockaddr*)&client;
	struct hostent *rem;

	/* First we initialize the socket that this monitor process will use for
	communication. */
	if( (sock = socket(AF_INET,SOCK_STREAM,0)) < 0 ){

		perror("Error on socket creation.");
		return 0;
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(port);

	int i =1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i)); 

	if(bind(sock,serverptr,sizeof(server)) < 0){

		cout << "Error on binding." << endl;
		return 0;

	}

	if(listen(sock,5) < 0){

		cout << "Error on listening." << endl;
		return 0;

	}

	if((newsocket = accept(sock,clientptr,&clientlen)) < 0){

		cout << "Error on accept." << endl;
		return 0;

	}

	/* We initialize the cyclic buffer. */
	initialize(&pool,cyclicBufferSize);

	/* Then we initialize all mutexes that we will use. */
	if(pthread_mutex_init(&lockBloomList, 0) != 0){

		perror("Error on mutex initialization.");
		return 0;
	}

	if(pthread_mutex_init(&mtx, 0) != 0){

		perror("Error on mutex initialization.");
		return 0;
	}

	if(pthread_mutex_init(&mutexAvailable, 0) != 0){

		perror("Error on mutex initialization.");
		return 0;
	}

	/* Then we initialize all condition variables that we will use. */
	if(pthread_cond_init(&condAvailable, 0) != 0){

		perror("Error on cond initialization.");
		return 0;
	}

	if(pthread_cond_init(&cond_nonempty, 0) != 0){

		perror("Error on cond initialization.");
		return 0;
	}

	if(pthread_cond_init(&cond_nonfull, 0) != 0){

		perror("Error on cond initialization.");
		return 0;
	}

	/* Threads array holds all extra threads that will be used in this certain monitor process. */
	pthread_t threads[numThreads];
	for(int i=0;i<numThreads;i++)
		/* Create all threads. */
		if(pthread_create(&(threads[i]),NULL,&MonitorThreads,NULL) != 0){

			perror("Error on thread creation.");
			return 0;

		}

	/* Put all file paths inside cyclic buffer. */
	while(totalFiles > 0){

		place(&pool, queue->get());
		totalFiles--;
		pthread_cond_signal(&cond_nonempty);
	}

	/* Wait for threads to finish updating BloomList. This statement is used in order to prevent 
	send outdated data to travelMonitor process. */
	while(limit < noFiles)
		pthread_cond_wait(&condAvailable,&mutexAvailable);

	BloomList* tempList = blooms;

	/* Here we send all bloomFilters that were filled during the initialization to parent. */
	while(tempList != NULL){

		sendBloom(newsocket,sizeOfBloom,tempList->getCells(),socketBufferSize,tempList->getVirus());
		tempList = tempList->getNext();
	}

	/* sep string is used in order to indicate the end of a message. It is used as an argument to sendMessage function. */
	char *sep = (char*)malloc(strlen("^")+1);
	strcpy(sep,"^");

	/* When all bloomFilters are sent, we send "end", in order for parent process to realize that monitor is done sending
	bloomFilters. */
	char endMessage[socketBufferSize];
	strcpy(endMessage,"end");

	sendMessage(newsocket,strlen(endMessage),endMessage,socketBufferSize,sep);

	char* tmp;

	int totalRequests =0;
	int totalAccepted =0;
	int totalRejected =0;

	/* The following while loop is used to manage the main functionality of monitors. */
	while(1){

		tmp = receiveMessage(newsocket,socketBufferSize,'^');

		/* We check if for first message key string : "travelRequest" was sent. In this case
		we must function as described. */
		if( !strcmp(tmp,"travelRequest") ){

			totalRequests++;

			/* First we get the data we need from fifo. */
			char *citizenID = receiveMessage(newsocket,socketBufferSize,'^');
			char *virusName = receiveMessage(newsocket,socketBufferSize,'^');

			char answer[9999];
			string date;

			/* If citizen exists in skip lists then send "YES" and the date he was vaccinated. */
			if( (date = blooms->isVaccinated(stoi(citizenID),virusName)) != " " ){

				strcpy(answer,"YES");
				sendMessage(newsocket,strlen(answer),answer,socketBufferSize,sep);
				char tmpDate[9999];
				strcpy(tmpDate,date.c_str());
				sendMessage(newsocket,strlen(tmpDate),tmpDate,socketBufferSize,sep);
			
				totalAccepted++;

			}else{

				/* Otherwise send "NO". */
				strcpy(answer,"NO");
				sendMessage(newsocket,strlen(answer),answer,socketBufferSize,sep);
		
				totalRejected++;	

			}

			free(citizenID);
			free(virusName);

		/* We check if for first message key string : "searchVaccinationStatus" was sent. In this case
		we must function as described. */
		}else if( !strcmp(tmp,"searchVaccinationStatus") ){

			/* We first read the data we need from fifo. */
			char* tmpCitizenID = receiveMessage(newsocket,socketBufferSize,'^');
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
				sendMessage(newsocket,strlen(tmpfm),tmpfm,socketBufferSize,sep);

				/* Then we send his age. */
				strcpy(tmpsm,sm.c_str());
				sendMessage(newsocket,strlen(tmpsm),tmpsm,socketBufferSize,sep);

				char send[9999];

				RecordsList *tmp = rl;

				/* And finally all his records for all viruses that this monitor has data for. */
				while(tmp != NULL){

					strcpy(send,tmp->getContent().c_str());
					sendMessage(newsocket,strlen(send),send,socketBufferSize,sep);

					tmp = tmp->getNext();
				}
			}

			/* Also send "end" message in order to indicate that we finished sending data. */
			char endMessage[socketBufferSize];
			strcpy(endMessage,"end");

			sendMessage(newsocket,strlen(endMessage),endMessage,socketBufferSize,sep);

			delete rl;
			free(tmpCitizenID);
			
		}else if(!strcmp(tmp,"addVaccinationRecords")){

			/* First we get from parent process the country which has updated data. */
			char* countryPath = receiveMessage(newsocket,socketBufferSize,'^');

			/* And we start the same process that we did at the start when initializing the
			BloomList. */
			DIR *d;
			struct dirent *dir;
			
			d = opendir(countryPath);

			while( (dir = readdir(d)) != NULL ){

				if( !strcmp(dir->d_name,".") || !strcmp(dir->d_name,"..")) continue;
				
				string filePath = countryPath;
				filePath += "/";
				filePath += dir->d_name;

				queue->insert(filePath);
				totalFiles++;

			}
			closedir(d);

			/* Delete previous cyclic buffer. */
			delete[] pool.data;
			/* Create a new one. */
			initialize(&pool,cyclicBufferSize);

			/* Wake up all threads. */
			for(int i=0;i<numThreads;i++)
				pthread_mutex_unlock(&mutexAvailable);

			noFiles = totalFiles;
			limit = 0;

			while(totalFiles > 0){

				place(&pool, queue->get());
				totalFiles--;
				pthread_cond_signal(&cond_nonempty);

			}

			while(limit < noFiles)
				pthread_cond_wait(&condAvailable,&mutexAvailable);

			tempList = blooms;

			/* And send all bloomFilters again in parent process. */
			while(tempList != NULL){

				sendBloom(newsocket,sizeOfBloom,tempList->getCells(),socketBufferSize,tempList->getVirus());
				tempList = tempList->getNext();

			}
		
			char endMessage[socketBufferSize];
			strcpy(endMessage,"end");

			/* Send end message in order to indicate that we are done sending bloom arrays. */
			sendMessage(newsocket,strlen(endMessage),endMessage,socketBufferSize,sep);

			free(countryPath);

		/* Exit message is sent. */
		}else{
			
			/* Create log file. */
			ofstream logfile;

			string logfileName;
			logfileName.append("log_file.");
			logfileName.append(to_string(getpid()));

			logfile.open(logfileName);

			DirectoriesList* tmpList = list;
			
			while(tmpList != NULL){

				logfile << tmpList->getCountry();
				logfile << "\n";
				tmpList = tmpList->getNext();
			}

			logfile << "TOTAL TRAVEL REQUESTS " << totalRequests;
			logfile << "\nACCEPTED " << totalAccepted;
			logfile << "\nREJECTED " << totalRejected;

			logfile.close();

			break;
		}

		free(tmp);
	}

	/* Make terminate flag true in order for threads to terminate. */
	terminateThreadsFlag = true;
	/* Wake up all threads in order for them to check the flag and terminate. */
	for(int i=0;i<numThreads;i++)
		pthread_mutex_unlock(&mutexAvailable);

	/* Wait for them to terminate and free up all their memory. */
	for(int i=0;i<numThreads;i++)
		pthread_join(threads[i],NULL);

	close(newsocket);

	pthread_mutex_destroy(&mtx);
	pthread_mutex_destroy(&mutexAvailable);
	pthread_mutex_destroy(&lockBloomList);
	
	pthread_cond_destroy(&cond_nonempty);
	pthread_cond_destroy(&cond_nonfull);
	pthread_cond_destroy(&condAvailable);
	
	free(sep);
	free(tmp);

	delete list;
	delete queue;
	delete blooms;
	delete citizens;
	delete[] pool.data;
}