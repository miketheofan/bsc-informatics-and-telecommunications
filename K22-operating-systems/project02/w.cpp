#include "w.hpp"

int prime1(int);
int prime2(int);
int prime3(int);
void signalFunction(int);
void findPrimes(int,int,primesFun,int);

int main(int argc,char **argv){

	int lb,ub,funAssignment , fd , parentid;

	if(argc != expectedArgsw){

		cout << "Wrong input.Exiting W" << endl;
		exit(1);
	}

	for(int i=0;i<argc;i++)

		if(!strcmp(argv[i],"-l"))
			lb = stoi(argv[++i]);
		else if(!strcmp(argv[i],"-u"))
			ub = stoi(argv[++i]);
		else if(!strcmp(argv[i],"-f"))
			funAssignment = stoi(argv[++i]);
		else if(!strcmp(argv[i],"-p"))
			fd = stoi(argv[++i]);
		else if(!strcmp(argv[i],"-i"))
			parentid = stoi(argv[++i]);

	switch(funAssignment){/*check function assignment given by L node and choose prime function that will use.*/

		case(1):{
			
			findPrimes(lb,ub,prime1,fd);
			break;
		}case(2):{

			findPrimes(lb,ub,prime2,fd);
			break;
		}case(3):{

			findPrimes(lb,ub,prime3,fd);
			break;
		}
	}

	close(fd);

	kill(parentid,SIGUSR1);/*when it finishes, it raises a SIGUSR1 signal to parent process.*/
	return 0;
}