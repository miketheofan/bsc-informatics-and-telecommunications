#include "functions.hpp"

int signals = 0;

int prime1(int n){

	int i;

	if(n==1) return(NO);
	
	for(i=2;i<n;i++)
		if(n%i == 0) return(NO);
	
	return(YES);
}

int prime2(int n){

	int i=0,limitup=0;
	limitup = (int)(sqrt((float)n));

	if(n==1) return(NO);
	
	for(i=2;i<=limitup;i++)
		if(n%i == 0) return(NO);
	
	return(YES);
}

/* Source: https://www.geeksforgeeks.org/sieve-of-eratosthenes/ */
int prime3(int n){

	bool *prime = (bool*) malloc(sizeof(bool) * n+1); 
    memset(prime, true, n+1); 
  
    for (int p=2; p*p<=n; p++) 
    { 
        // If prime[p] is not changed, then it is a prime 
        if (prime[p] == true) 
        { 
            // Update all multiples of p greater than or  
            // equal to the square of it 
            // numbers which are multiple of p and are 
            // less than p^2 are already been marked.  
            for (int i=p*p; i<=n; i += p) 
                prime[i] = false; 
        } 
    } 
   
    if (prime[n] == true){

    	free(prime);
        return YES;
    }
    return NO; 
}

/*Following function uses primesFun pointer to get a function,find primes between lb,ub
and writes them to a pipe given*/
void findPrimes(int lb,int ub,primesFun function,int fd){

	double tStart,tFinish,tNumber = 0,total,curNumberTime;
	struct tms tb1,tb2,tb3;
	double ticspersec;
	int sum = 0;
	string entryNumber,entryTotalTime,tempNumber,tempTime;

/*The function also uses time calculating, as it is in the code given at website*/
	ticspersec = (double)sysconf(_SC_CLK_TCK);

	tStart = (double)times(&tb1);
	tNumber = tStart;

	for(int i=lb;i<=ub;i++){

		if(function(i)){

			curNumberTime = ((double)times(&tb3) - tNumber)/ticspersec;
			
			if(write(fd, &curNumberTime ,  sizeof(curNumberTime)) == -1)/*First inserts time*/
				cout << "Error writting in pipe." << endl;

			if(write(fd, &i ,  sizeof(i)) == -1)/*Then inserts number*/
				cout << "Error writting in pipe." << endl;

			sum++;

			tNumber = (double)times(&tb3);
		}
	}

	tFinish = (double)times(&tb2);

	total = (tFinish-tStart)/ticspersec;
	if(write(fd,&total,sizeof(total)) == -1)/*At the end inserting total time that the function needed*/
		cout << "Error writting in pipe." << endl;
}

void signalFunction(int signo){

	if(signo == SIGUSR1) signals++;
}