#include "saladmaker.hpp"

string messageFormat(int,int,string);

int main(int argc,char** argv){

	srand(time(NULL));

	string globalFileName , logfileName;
	ofstream logfile;
	int *chefID , mantime , shmid , t1 , t2 ,makerOrder , *numOfSlds , id , *salads;
	sem_t *saladmaker , *check;

	if(argc != expectedArgs){

		cout << "Wrong input.Exiting program." << endl;
		return 0;
	}

	for(int i=0;i<argc;i++)

		if(!strcmp(argv[i],"-t1"))
			t1 = stoi(argv[++i]);
		else if(!strcmp(argv[i],"-t2"))
			t2 = stoi(argv[++i]);
		else if(!strcmp(argv[i],"-s")) /* Shared memory segment's ID. */
			shmid = stoi(argv[++i]);
		else if(!strcmp(argv[i],"-o")) /* Order that current saladmaker has (1,2,3) */
			makerOrder = stoi(argv[++i]);

	numOfSlds = (int*) shmat(shmid,(void*)0,0); /* Assign number of salads segment. */
	chefID = numOfSlds+4; /* Assign chef's id segment. */

	Logfile log(*chefID); /* Initialize log file , so that this process can write its messages not only in its personal one, but in the general log file too. */

	/* Depending on current saladmaker's order, open the corresponfing named semaphores , assign the corresponding salads count and the corresponding process id that we need to pass to chef process. */ 
	switch(makerOrder){

		case(1):{

			saladmaker = sem_open("saladmaker1",0);
			check = sem_open("check1",0);
			salads = numOfSlds+1;

			break;

		}case(2):{

			saladmaker = sem_open("saladmaker2",0);
			check = sem_open("check2",0);
			salads = numOfSlds+2;

			break;

		}case(3):{

			saladmaker = sem_open("saladmaker3",0);
			check = sem_open("check3",0);
			salads = numOfSlds+3;

			break;

		}
	}

	id = getpid();/*  Initialize saladmaker's id segment in shared memory, with current process' id. */

	logfileName.append("saladmaker"+to_string(makerOrder)+"log"+to_string(id)+".txt");
	logfile.open(logfileName); /* Open log file that current saladmaker will use to write his messages. */

	mantime = rand() % t2 + t1; /* Calculate cooking time that current saladmaker will need in order to cook a salad. */

	sem_post(check); /* Post semaphore that checks if saladmaker is currently available. This action will "wake up" chef in order for him to provide the vegetables that he has already chosen. */
	
	logfile << messageFormat(id,makerOrder,message1);
	log.write(messageFormat(id,makerOrder,message1));
	
	sem_wait(saladmaker); /* Wait here is used to get semaphore's value from 1 to 0 in order for saladmaker to block when he finishes his cooking. */

	while(1){

		if( (*numOfSlds) <= 0) break; /* Check if any salads are left to be cooked. */

		logfile << messageFormat(id,makerOrder,message2);
		log.write(messageFormat(id,makerOrder,message2));
		
		logfile << messageFormat(id,makerOrder,message3);
		log.write(messageFormat(id,makerOrder,message3));
		
		sleep(mantime); /* Take mantime to cook the salad. */
		
		logfile << messageFormat(id,makerOrder,message4);
		log.write(messageFormat(id,makerOrder,message4));
		
		(*salads)++; /* Update the varibale that counts the amount of salads this current saladmaker has cooked. */

		if( --(*numOfSlds) <= 0 ) break; /* Checks again and also updates number of salads variable. */

		logfile << messageFormat(id,makerOrder,message1);
		log.write(messageFormat(id,makerOrder,message1));

		sem_post(check); /* Post here is used to indicate that current saladmaker is available to get vegetables. */
		sem_wait(saladmaker); /* Wait here is used to block the saladmaker if he has no salad to cook. */

	}

	logfile.close(); /* Close saladmaker's log file. */

	/* Close named semaphores. */
	sem_close(saladmaker);
	sem_close(check);

	return 0;
}