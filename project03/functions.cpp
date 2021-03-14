#include "functions.hpp"

/* Following function calculates the current time, following the format that was given here: https://www.alexdelis.eu/k22/formatted-output.f20-prj3-v1.pdf. */
string dateFormat(){

	stringstream streamH , streamM , streamS , streamMS;
	long int ms;
	double hours,minutes,seconds;
	time_t rawtime;
	struct tm *info;
	struct timeval tp;
	string format;

	gettimeofday(&tp,NULL);

	ms = (tp.tv_usec / 1000) % 60;
	streamMS << setw(2) << setfill('0') << ms;

	time(&rawtime);
	info = localtime(&rawtime);

	hours = info->tm_hour;
	streamH << setw(2) << setfill('0') << hours;
	minutes = info->tm_min;
	streamM << setw(2) << setfill('0') << minutes;
	seconds = info->tm_sec;
	streamS << setw(2) << setfill('0') << seconds;

	format.append("["+streamH.str()+":"+streamM.str()+":"+streamS.str()+":"+streamMS.str()+"]");

	return format;
}

/* Following function uses dateFormat and finally adding all the "pieces" in order to provide the messages as formatted here: https://www.alexdelis.eu/k22/formatted-output.f20-prj3-v1.pdf. */
string messageFormat(int id,int order,string message){

	string format , idFormat , oFormat , mFormat;

	idFormat.append("["+to_string(id)+"]");	

	if(order <=3) oFormat.append("[Saladmaker"+to_string(order)+"]");
	else oFormat.append("[Chef]");
	
	mFormat.append("["+message+"]");

	format.append(dateFormat()+idFormat+oFormat+mFormat+"\n");

	return format;
}

/* Following function gets an integer and returns how many digits the integer have. */
int getDigits(int number){

	int length = 1;

	while(number /= 10) length++;

	return length;
}