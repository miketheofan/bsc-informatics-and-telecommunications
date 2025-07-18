/*This program uses a function for multiplication and one more for the calculations in between brackets.*/
/*They are all included in main.*/
#include <stdio.h>
#include <ctype.h>
#define ERROR -1
int *next;/*This pointer is used to save the last character that was entered before the function ended on multiplication().*/
int error;/*This variable is used to indicate what was the mistake that was typed in the program.*/
	  /*1 for character mistake,2 for not closed bracket,3 for operator as last character before the program ended.*/
int mults;/*This variable is used to check for more than one multiplication operators in a row.*/
int faults;/*This variable is used to indicate errors found.*/
int brackets(void){/*This is a function that calculates the total sum of numbers between the brackets*/
	int multiplication(int);
	int temp,mults=0,brackSum,current,  number=0, currentSum=0, oper=1, flag, times= 0, minusCounter,multSum;
  	current=getchar();
  	while(isspace(current)){/*While a white character is being entered,we ignore it and read the next one.*/
		current=getchar();
	}
	while(current=='-'){/*While the operator '-' is being entered we calculate the sign of the next number by multiplying by -1.*/
		oper*=-1;
		current=getchar();
  	}
/*While a digit is being entered with the following method we calculate the full-digit number.*/
  	while(isdigit(current)){
		currentSum=currentSum*10+current-'0';
		temp=currentSum*oper;
		current=getchar();
	}
	currentSum*=oper;/*We now save the current sum by multiplying the operator (for the sign of the number) with the full-digit number.*/
  	oper=1;/*Now we give the operator variable its' starting value.*/
  	while(current!=')'){
		multSum=0;
/*if we need to multiply we call the corresponding function and add the number that was returned by the function to the current sum.*/
                if(current=='*'){
        	        multSum=multiplication(temp);
  	                if(multSum==ERROR){
                                return ERROR;
                        }
                        currentSum+=multSum-temp;
/*With the temp variable we store the last number that was entered in the programm and use it for the multiplication.*/
/*we don't include the temp number in the current sum because we only needed it for the function.*/
                        temp=multSum;
                        current=*next;
                        continue;
                }
/*if an opening bracket is entered,we store the total sum inside the brackets and add that to the current sum (we also calculate the sign
					before the number).*/
		if(current=='('){
			brackSum=brackets();
			temp=brackSum;
			currentSum+=brackSum*oper;
		}
/*If we need to add or subtract we update the corresponding variable for later use in the function.*/
                if ( current == '+' || current == '-' ) {
                        flag = 1;
                }
/*Now we change the current sum either by substracting or adding.*/
                if(flag==1&&minusCounter==0){
                        currentSum+=(oper*number);
                        number=0;
                        oper=1;
                        flag=0;
		}
		/*The minus counter is used for the condition above.*/
		/*The following conditions are used to mainly check for errors entered.*/
                if ( current == '-' ) {
			mults=0;
                        minusCounter++;
                        oper *= -1;
                }else if(current=='*'){
			mults++;
			if(mults>=2){
				if(!error)
					error=1;
			}
                        minusCounter=0;
                }else if (isdigit(current)) {
                        minusCounter=0;
                        number = number * 10 + current - '0';
                        temp=(oper*number);
                }else if(current=='\n'){
			if(!error)
				error=2;
			return ERROR;
		}else if (isspace(current)) {
                        minusCounter=0;
                }else if(current=='+'){
			mults=0;
                        minusCounter=0;
		}else if(current=='('){
			minusCounter=0;
			mults=0;
		}else {
			if(!error)
				error=1;
                        minusCounter=0;
                        return ERROR;
                }
		current = getchar();
  	}
	currentSum+=oper*number;
/*If the function returns ERROR,a mistake was entered,otherwise it returns the current sum.*/
	return currentSum;
}
/*The next function is used to multiply numbers when the corresponding operator is entered.*/
/*The function needs a parameter for the first number of the calculation that needs to be multiplied.*/
int multiplication(int starting_point){
	int mults=0,brackSum,returned=0,oper=1,current;
	int temp=0;
	returned=starting_point;
	current=getchar();
/*If a multiply operator is entered,we return error because there can't be two in a row.*/
	if(current=='*'){
		if(!error)
			error=1;
		faults=1;
	}
/*Ignoring white characters again.*/
	while(isspace(current)){
		current=getchar();
	}
	while(current=='-'){
		oper*=-1;
		current=getchar();
	}
/*If an opening bracket is entered we calculate the sum by calling the function and multiply it with the sum that is going to be returned*/
					/*In that case is the variable "returned".*/
	if(current=='('){
		brackSum=brackets();
		if(brackSum==ERROR){
			if(error==2)
			return ERROR;
		}
		returned*=oper*brackSum;
		current=getchar();
		next=&current;
		return returned;
	}
	while(isspace(current)&&current!='\n'){
		current=getchar();
	}
/*In the next two conditions we calculate the sign of the next number.*/
	while(current=='+'){
		oper*=1;
		current=getchar();
	}
	while(current=='-'){
		oper*=-1;
		current=getchar();
	}
/*Here we calculate the full-digit number with the digits entered.*/
	if(isdigit(current)){
		while(isdigit(current)){
			temp=temp*10+current-'0';
			current=getchar();
		}
		returned*=(oper*temp);
		oper=1;
		temp=0;
	}
	next=&current;/*Here we save the last character entered.*/
/*If anything else is being entered we return error.*/
	if(current!='('&&current!=')'&&!isdigit(current)&&!isspace(current)&&current!='-'&&current!='*'&&current!='+'){
		if(!error)
			error=1;
		return ERROR;
	}
/*Otherwise we return the multiplied numbers.*/
	return returned;
}
/*Our main function starts here.*/
int main() {
        int tempCurrent,brackSum,current,  number, currentSum, oper, flag, times= 0,minusCounter,multSum;
	int temp=0;
	current=getchar();
	while (current != EOF) {/*While the EOF character is not being entered we continue.*/
		temp=0;
		error=0,mults=0,faults = 0, number = 0, currentSum = 0, oper = 1, flag = 0,minusCounter=0,times++;
/*If any character other than the sign or open bracket is entered we return error.*/
		if(ispunct(current)&&current!='('&&current!='+'&&current!='-'){
			faults=1;
			if(!error)
				error=1;
		}
/*Ignoring white characters...*/
		while (isspace(current)) {
			current = getchar();
		}
/*Calculating the sign of the following number.*/
		while ( current == '-'){
			oper*=-1;
			current = getchar();
		}
/*Making the full-digit number...*/
       		while (isdigit(current)) {
       			currentSum = currentSum * 10 + current - '0';
         		temp=(oper*currentSum);
			current = getchar();
       		}
       		currentSum *= oper;/*Saving the current sum.*/
       		if(currentSum){
			oper = 1;
		}
/*The program will end when either the space character is entered or the EOF,which will end the previous loop too.*/
/*If space is entered we print the result*/
		while ( current != '\n' && current != EOF ) {
			multSum=0,brackSum=0;
			while(isspace(current)){
				current=getchar();
			}
			while(current=='-'){
				oper*=-1;
				current=getchar();
			}
/*Here we call the brackets function and adding the sum of numbers inside the brackets to the current sum.*/
			if(current=='('){
				brackSum=brackets();
				if(brackSum==ERROR){/*If ERROR is returned we save error.*/
					faults=1;
					if(error==2){
						current=getchar();
						break;
					}
				}
				currentSum+=(oper*brackSum);
				if(!temp)/*Always saving the last number for further processing.*/
					temp=oper*currentSum;
			}
/*If the mutiplying character is entered we call the multiplication function by giving as a parameter the last number that we saved,
		and add the number that the function returns to the current sum.*/
			if(current=='*'){
				if(mults==2){
					faults=1;
					if(!error)
						error=1;
				}
				multSum=multiplication(temp);
	/*If it returns ERROR we save error.*/
				if(multSum==ERROR){
					faults=1;
				}
				currentSum+=multSum-temp;
				temp=multSum;
				current=*next;
				continue;
			}
	/*Same method to substract and add numbers as we did in the brackets function.*/ 
			if ( current == '+' || current == '-' ) {
				flag = 1;
			}
			if(flag==1&&minusCounter==0){
				currentSum+=(oper*number);
				number=0;
				oper=1;
				flag=0;
			}
			if ( current == '-' ) {
				minusCounter++;
				oper *= -1;
			}else if(current=='*'){
				mults++;
				if(mults>=2){
					faults=1;
					if(!error)
						error=1;
				}
				minusCounter=0;
			}else if (isdigit(current)) {
				minusCounter=0;
				number = number * 10 + current - '0';
				temp=(oper*number);
			}else if (isspace(current)) {
				minusCounter=0;
			}else if(current=='+'){
				mults=0;
				minusCounter=0;
			}else if(current=='('){
				mults=0;
				minusCounter=0;
			}else {
				if(!error)
					error=1;
				minusCounter=0;
				faults = 1;
			}
			tempCurrent=current;
			current=getchar();
/*The following condition checks if the current calculation entered with a digit as a character,
			in which case returns error.*/
			if(tempCurrent=='-'||tempCurrent=='+'||tempCurrent=='*'){
				if(current=='\n'){
					faults=1;
					if(!error)
						error=3;
				}
			}
		}
		/*If space character was entered it exits the loop and if EOF was entered it stops the programm.*/
		if ( current == EOF ) {
                        break;
		}
	/*Here we calculate the last operation before space character was entered.*/
		currentSum += oper * number;
/*Last but not least with this method we print to the screen the result of the operations.*/
		printf("Result %d: ",times);
		if (faults) {
			if(error==1){
				printf("Unexpected character\n");
			}else if(error==2){
				printf("Missing closing parenthesis\n");
			}else if(error==3){
				printf("Unexpected end of input\n");
			}else{/*This else condition is here only because of the mess between -1=-1 & ERROR=-1.*/
				printf("%d\n",currentSum);
			}
		} else {
			printf("%d\n", currentSum);
		}
	}
}
/*End of program.*/
