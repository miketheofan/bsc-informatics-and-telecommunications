#include "chef.hpp"

string messageFormat(int,int,string);
int getDigits(int);

int main(int argc,char **argv){

	srand(time(NULL));

	int *chefID , retval , id , shmid , err , numOfSlds , mantime , *salads1 , *salads2 , *salads3 , *saladsPointer;
	sem_t *saladmaker1 , *saladmaker2 , *saladmaker3 , *check1 , *check2 , *check3;
	string globalFileName , logfileName , ingredient1 , ingredient2 , previousIngredient1 , previousIngredient2 , ingredients[] = { "tomato" , "onion" , "pepper" };
	ofstream logfile;

	id = getpid(); /* Initialize chef's process id in order to pass it as a segment through shared memory segment so that saladmaker processes can have an access to log file. */
	Logfile log(id);

	if(argc != expectedArgs){

		cout << "Wrong input.Exiting program." << endl;
		exit(1);
	}

	for(int i=0;i<argc;i++)

		if(!strcmp(argv[i],"-n")) /* Number of salads. */
			numOfSlds = stoi(argv[++i]);
		else if(!strcmp(argv[i],"-m")) /* Mantime that chef has to take a rest. */
			mantime = stoi(argv[++i]);

	shmid = shmget(IPC_PRIVATE , IPC_SIZE ,0666);/*  Make  shared  memory  segment  */
	if (shmid ==  -1) perror ("Creation");
	else  printf("Allocated. %d\n",(int)shmid);

	saladsPointer = (int*) shmat(shmid,(void*) 0,0); /* Attach number of salads segment in shared memory segment. */
	if (*(int *) saladsPointer ==  -1) perror("Attachment.");

	*saladsPointer = numOfSlds; /* Give in number of salads segment the value that was given as an argument. */

	salads1 = saladsPointer+1; /* Variable that holds the total amount of salads salad maker 1 cooked in total. */
	*salads1 = 0; /* Initialize it as zero. */
	
	salads2 = saladsPointer+2; /* Variable that holds the total amount of salads salad maker 2 cooked in total. */
	*salads2 = 0; /* Initialize it as zero. */
	
	salads3 = saladsPointer+3; /* Variable that holds the total amount of salads salad maker 3 cooked in total. */
	*salads3 = 0; /* Initialize it as zero. */

	chefID = saladsPointer+4; /* Variable that holds the id of chef process. */
	*chefID = id; /* Initialize it with this process' process id. */

	logfileName.append("cheflog"+to_string(id)+".txt");
	
	logfile.open(logfileName); /* Open chef's log file where he will write his messages. */

	/* Create named semaphores. saladmaker semaphores are used to wake up saladmakers and block them whenever they do not have the vegetables they need to cook a salad. */
	/* check semaphores are useful for chef process in order to know when a saladmaker is cooking and when not, in order to give him next order. */
	saladmaker1 =sem_open("saladmaker1" , O_CREAT | O_EXCL , S_IRUSR|S_IWUSR , 0);
	if(errno == EEXIST){

		perror("Semaphore already exists.");
		exit(1);
	}
	saladmaker2 =sem_open("saladmaker2" , O_CREAT | O_EXCL , S_IRUSR|S_IWUSR , 0);
	if(errno == EEXIST){

		perror("Semaphore already exists.");
		exit(1);
	}
	saladmaker3 =sem_open("saladmaker3" , O_CREAT | O_EXCL , S_IRUSR|S_IWUSR , 0);
	if(errno == EEXIST){

		perror("Semaphore already exists.");
		exit(1);
	}
	check1 =sem_open("check1" , O_CREAT | O_EXCL , S_IRUSR|S_IWUSR , 0);
	if(errno == EEXIST){

		perror("Semaphore already exists.");
		exit(1);
	}
	check2 =sem_open("check2" , O_CREAT | O_EXCL , S_IRUSR|S_IWUSR , 0);
	if(errno == EEXIST){

		perror("Semaphore already exists.");
		exit(1);
	}
	check3 =sem_open("check3" , O_CREAT | O_EXCL , S_IRUSR|S_IWUSR , 0);
	if(errno == EEXIST){

		perror("Semaphore already exists.");
		exit(1);
	}

	/* Intialization of named semaphores. */
	retval = sem_init(saladmaker1,1,0);
	if(retval != 0){

		perror("Could not initialize semaphore.");
		exit(1);
	}
	retval = sem_init(saladmaker2,1,0);
	if(retval != 0){

		perror("Could not initialize semaphore.");
		exit(1);
	}
	retval = sem_init(saladmaker3,1,0);
	if(retval != 0){

		perror("Could not initialize semaphore.");
		exit(1);
	}
	retval = sem_init(check1,1,0);
	if(retval != 0){

		perror("Could not initialize semaphore.");
		exit(1);
	}
	retval = sem_init(check3,1,0);
	if(retval != 0){

		perror("Could not initialize semaphore.");
		exit(1);
	}
	retval = sem_init(check3,1,0);
	if(retval != 0){

		perror("Could not initialize semaphore.");
		exit(1);
	}

	while(numOfSlds){ /* While there are more salads that need to be cooked. */

		/* Pick two random ingredients. */
		ingredient1 = ingredients[rand()%3];
		ingredient2 = ingredients[rand()%3];

		if(ingredient1 == ingredient2) continue; /* If same ingredient was chosen twice ,continue with next loop. */
		/* If same ingredients as in previous loop where chosen , also continue to next loop. */
		else if( (ingredient1 == previousIngredient1 && ingredient2 == previousIngredient2) || (ingredient1 == previousIngredient2 && ingredient2 == previousIngredient1)) continue;
	
		/* Update previous ingredients. */
		previousIngredient1 = ingredient1;
		previousIngredient2 = ingredient2;

		if( (ingredient1 == "tomato" && ingredient2 == "pepper") || (ingredient1 == "pepper" && ingredient2 == "tomato") ){

			logfile << messageFormat(id,4,message1tp);
			log.write(messageFormat(id,4,message1tp));

			sem_wait(check1); /* Wait is used in order to check if saladmaker is already cooking a salad, so we have to wait for him to finish and get the new vegetables. */
			
			logfile << messageFormat(id,4,message2a);
			log.write(messageFormat(id,4,message2a));
			
			sem_post(saladmaker1); /* Post is used to "wake up" the salad maker in order for him to cook a salad. */

		}else if( (ingredient1 == "tomato" && ingredient2 == "onion") || (ingredient1 == "onion" && ingredient2 == "tomato") ){

			logfile << messageFormat(id,4,message1to);
			log.write(messageFormat(id,4,message1to));

			sem_wait(check2);

			logfile << messageFormat(id,4,message2b);
			log.write(messageFormat(id,4,message2b));
			
			sem_post(saladmaker2);

		}else if( (ingredient1 == "onion" && ingredient2 == "pepper") || (ingredient1 == "pepper" && ingredient2 == "onion") ){

			logfile << messageFormat(id,4,message1po);
			log.write(messageFormat(id,4,message1po));

			sem_wait(check3);
			
			logfile << messageFormat(id,4,message2c);
			log.write(messageFormat(id,4,message2c));
			
			sem_post(saladmaker3);

		}

		numOfSlds--;
		logfile << messageFormat(id,4,message3);		
		log.write(messageFormat(id,4,message3));
		sleep(mantime); /* Take a rest for as long as you were provided from the arguments. */
	}

	/* Post here is used to "wake up" any process that was blocked up till this point. */
	sem_post(saladmaker1);
	sem_post(saladmaker2);
	sem_post(saladmaker3);

	sleep(5); /* Sleep is used to wait for "slow" saladmakers to finish, in order to provide correct results. */

	cout << "Total salads made are: " << (*salads1)+(*salads2)+(*salads3) << endl;
	cout << "Salad maker 1 made: " << *salads1 << endl;
	cout << "Salad maker 2 made: " << *salads2 << endl;
	cout << "Salad maker 3 made: " << *salads3 << endl;

	cout << endl;
	
	cout << "Time intervals: (in increasing order)" << endl;
	log.findConcurrent(getDigits(id));

	/* Unlink named semaphores. */
	sem_unlink("saladmaker1");
	sem_unlink("saladmaker2");
	sem_unlink("saladmaker3");
	sem_unlink("check1");
	sem_unlink("check2");
	sem_unlink("check3");

	/* Close named semaphores. */
	sem_close(saladmaker1);
	sem_close(saladmaker2);
	sem_close(saladmaker3);
	sem_close(check1);
	sem_close(check2);
	sem_close(check3);

	/* Close log file. */
	logfile.close();

	/*  Remove  segment  */
	err = shmctl(shmid, IPC_RMID , 0);
	if (err ==  -1) perror ("Removal.");
	else  printf("Removed. %d\n", (int)(err));

	return 0;
}