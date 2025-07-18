#include "myprimes.hpp"

int prime1(int);
int prime2(int);
int prime3(int);
void signalFunction(int);
void findPrimes(int,int,primesFun);

extern int signals;

int main(int argc,char **argv){

	signal(SIGUSR1,signalFunction);/*Catch signals type SIGUSR1*/

	int lb,ub,children;

	if(argc != expectedArgs){

		cout << "Wrong input.Exiting program..." << endl;
		return 0;
	}

	for(int i=0;i<argc;i++)/*Get the data we need from argv*/

		if(!strcmp(argv[i],"-l"))//lb
			lb = stoi(argv[++i]);
		else if(!strcmp(argv[i],"-u"))//ub
			ub = stoi(argv[++i]);
		else if(!strcmp(argv[i],"-w"))//amount of children
			children = stoi(argv[++i]);

	int assign = 1 , tempLowest = lb , **fd = new int*[children] , timesCounter = 0;
	pid_t pid , cProcessid = getpid();/*We also pass as argument to all children nodes, the parent id in order for workers to raise SIGUSR1*/
	List *list = new List;
	char** vector = (char**)malloc(sizeof(char*)*(expectedArgsi+1));
	string temp_lb , temp_ub , temp_children , temp_fd , temp_assign , temp_id;

	for(int i=0;i<expectedArgsi;i++)/*Allocate an array in which we will store the arguments we need to create L nodes*/
		vector[i] = (char*)malloc(defaultArgsSize);
	vector[expectedArgsi] = NULL;

	for(int i=0;i<children;i++){/*Create pipes that we will use for children nodes in order to communicate.Every child gets
		a different pipe by all the other ones.*/

		fd[i] = new int[2];
		pipe(fd[i]);
	}

	for(int i=0;i<children;i++){

		if(assign > numberOfFunctions){ /*if current assign value is out of bounds*/

			if(assign % numberOfFunctions == 0) assign = 3; /*check cases like 6,9,12th number where they have to go to 3rd function.*/
			else assign = (assign) % numberOfFunctions;	/*otherwise cases like 5,8,11th number where they have to go to %2=2function and cases like*/
														/* 4,7,10th number where they have to go to 1st function*/ 		
		}

/*We pass the data we need to execute ./l.cpp in vector array*/
		temp_lb = to_string(tempLowest);
		temp_children = to_string(children);
		temp_assign = to_string(assign);
		temp_id = to_string(cProcessid);		
		if(i != children-1)
			temp_ub = to_string(tempLowest + (ub-lb)/children );
		else
			temp_ub = to_string(ub);

		strcpy(vector[0],"./l");
		strcpy(vector[1],"-l");//lb
		strcpy(vector[2],temp_lb.c_str());
		strcpy(vector[3],"-u");//ub
		strcpy(vector[4],temp_ub.c_str());
		strcpy(vector[5],"-w");//children
		strcpy(vector[6],temp_children.c_str());
		strcpy(vector[7],"-i");//parent id
		strcpy(vector[8],temp_id.c_str());
		strcpy(vector[9],"-a");//primes function assignment
		strcpy(vector[10],temp_assign.c_str());
		
		pid = fork();//fork

/*If child process, then execute with execvp and vector already filled up , the l node that is represented by l.cpp*/
		if(pid == 0) {

			close(*(fd[i]+0));
			temp_fd = to_string(*(fd[i]+1));

			strcpy(vector[11],"-p");//pipe that will use
			strcpy(vector[12],temp_fd.c_str());

			if(execvp("./l",vector) == -1)
				cout << "Error creating L process." << endl;
		}
		tempLowest = tempLowest + (ub-lb)/children;/*Create next [lb,ub]*/
		assign+=children;/*add children number to assign, to make sure next number will get the correct prime function as*/
						/*children-amount of numbers have already been assigned till this point with current value of variable assign.*/
	}

/*When all child process have finished, we start reading from their pipes and insert the results to a list.*/
	int size;

	double *times = new double[children*children];/*This is an array that we will use to store workers timers*/

	for(int i=0;i<children*children;i++)
		times[i] = -1;
	for(int i=0;i<children;i++){/*For every child, open its pipe and read from it.*/

		if( read(*(fd[i]+0),&size,sizeof(size)) <= 0 )/*First read total size of data L node is sending.*/
			cout << "Error in reading from pipe." << endl;

		double batch[size][2];/*Create an array with size that we read just before that*/

		if( read(*(fd[i]+0),&batch,sizeof(batch)) > 0 ){/*then read the array*/
			
			for(int j=0;j<size;j++)
			
			/*Here we check if data is a number or a total time that a worker needed to execute.*/	
				if( (int)batch[j][0] != totalTimeValue )
					list->insert( (int)batch[j][0] , batch[j][1] );
				else
					*(times + (timesCounter++) ) = batch[j][1];
		}
	}

	double max = times[0] , min = times[0] , signalsFloat = signals/(children*children);
	for(int i=1;i<children*children;i++){

		if(times[i] > max) max = times[i];
		if(times[i] < min) min = times[i];
	}
	
	cout << "Primes in " << "[" << lb << "," << ub << "] are: ";
	list->print();
	cout << "Minimum Time for Workers: " << setprecision(4) << min << endl;
	cout << "Maximum Time for Workers: " << setprecision(4) << max << endl;	
	cout << "Num of USR1 received: " << setprecision(4) << signalsFloat << endl;
	for(int i=0;i<children*children;i++)
		cout << "Time for W" << i << ":" << times[i] << endl;

	for(int i=0;i<expectedArgsi+1;i++)
		free(vector[i]);
	free(vector);

	for(int i=0;i<children;i++)
		delete[] fd[i];
	delete[] fd;
	
	delete[]times;
	delete list;	
	return 0;
}