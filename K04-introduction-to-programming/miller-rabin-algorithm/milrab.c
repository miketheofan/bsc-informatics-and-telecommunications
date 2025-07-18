//Result:904533 with the Deterministic Method
//and 904533 with the Miller-Rabin method
//Execution time:47 seconds with the Deterministic algorithm on puTTY
//and 2.93 seconds with the Miller-Rabin algorithm on puTTY

#include <stdio.h>
#include <time.h>
#define MINNUM 3990000000
#define MAXNUM 4010000000
int main(){
	printf("Checking range [%ld,%ld] for primes\n",MINNUM,MAXNUM);
	clock_t start_t,end_t;	/*These are the variables for the time that the programm will take to execute*/ 
	int out,times,check2,primes2=0,j,primes=0,check=0;
	double total_t;
	unsigned long a,tempa;
	unsigned int Num,tempNum,curNum,posNum,temp;
	long long x;
	unsigned long long tempx;
	start_t=clock();	/*setting the time that the loop started*/
	for(curNum=MINNUM+1;curNum<=MAXNUM;curNum=curNum+2){	/*checking only for odd numbers because the rest cannot be prime numbers*/
			if(curNum%3!=0){ 	/*check if the number is a divisor of number 3*/
				for(posNum=5;posNum*posNum<=curNum;posNum=posNum+6){	/*this way we check only for the least amount of*/
					if(curNum%posNum==0){				/*divisors we can be accurate with*/
						check=1;
						break;
					}
					if(curNum%(posNum+2)==0){
						check=1;
						break;
					}
				}   /*with those two if statements we managed to reduse the number of loops for a certain amount of numbers*/
				if(check==0)	/*if we found no divisors for this number*/
					primes++;	/*we add one to the variable for prime numbers*/
				check=0;
			}
	}
	end_t=clock();	/*setting the time that the loop ended*/
	total_t=(double)(end_t-start_t)/CLOCKS_PER_SEC;	/*we calaculate the total execution time*/
	printf("Deterministic algorithm : Found %d primes in %.2f seconds.\n",primes,total_t);


					/*|START OF SECOND ALGORITHM|*/


	start_t=clock();	/*starting the Miller-Rabin algorithm:*/
	for(Num=MINNUM+1;Num<=MAXNUM;Num+=2){	/*for all the odd numbers between the range given:*/
		if((Num%3!=0)&&(Num%5!=0)&&(Num%7!=0)){	/*not including the divisors of number 3 or 5 or 7*/ 
			out=0;
			check2=0;
			x=1;
			times=0;
			tempNum=Num-1;
			while(tempNum%2==0){	/*trying to build ( 2 ^ times ) mod tempNum*/
				times++;
				tempNum/=2;
			}
			a=2;
			while(a){	/*for every a between the range {2,7,61}*/
				temp=tempNum;
				out=0;
				x=1;
				tempa=a;
				while(temp>0){	/*setting x = ( a ^ tempNum ) mod Num*/
					if(temp%2==1)
						x=(x*tempa)%Num;
					temp/=2;
					tempa=(tempa*tempa)%Num;
				}
				if((x==1)||(x==(Num-1))){	/*if any of these two conditions is True,we go to the next a,*/
					switch(a){		/*if there is one.*/
						case 2 : {
							a=7;
							break;
						}case 7 : {
							a=61;
							break;
						}case 61 : {
							a=0;
							break;
						}
					}
					out=1;
					continue;
				}
				tempx=x;
				for(j=1;j<=(times-1);j++){	/*building x = ( x * x ) mod Num*/
					tempx=(tempx*tempx)%Num;
					if(tempx==(Num-1)){	/*again if this condition is True we go to the next a,if there is one*/
						out=1;	/*this variable indicates that we got out from the loop*/
						break;
					}
				}
				switch(a){	/*switching to the next a*/
					case 2 : {
						a=7;
						break;
					}case 7 : {
						a=61;
						break;
					}case 61 : {
						a=0;
						break;
					}
				}
				if(out==1)	/*if we got out from the previous loop,continue with next a*/
					continue;
				else{
					check2++;	/*otherwise,we find composite and should break out of the loop*/
					break;
				}
			}
			if(check2==0)	/*if we didn't break out of the loop at least once,the number is prime*/
				primes2++;
		}
	}
	end_t=clock();
	total_t=(double)(end_t-start_t)/CLOCKS_PER_SEC;
	printf("Miller-Rabin algorithm :  Found %d primes in %.2f seconds.\n",primes2,total_t);
}
