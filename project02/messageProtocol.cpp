#include "messageProtocol.hpp"

/* The following function is used from parent process to send the value of buffer size
to each monitor. In my implementation the first message is sent in buffer size: sizeof(int),
since monitors don't know the value of bufferSize at the beggining of their execution. */
void sendFirstMessage(int fd,int message){
	write(fd,&message,firstMessageBuffer);
}

void receiveFirstMessage(int fd,int *message){
	read(fd,message,firstMessageBuffer);
}

/* The following function is used to send a bloomArray from a monitor process to travelMonitor process. */
void sendBloom(int fd,int messageSize,char* message,int bufferSize,string country){

	char* sep = (char*)calloc(9999,sizeof(char));
	strcpy(sep,"^");

	/* First we send the array itself. */
	sendMessage(fd,messageSize,message,bufferSize,sep);
	
	/* Then we send the country that corresponds to this certain bloomArray. */
	char tmpCountry[9999];
	strcpy(tmpCountry,country.c_str());
	sendMessage(fd,strlen(tmpCountry),tmpCountry,bufferSize,sep);

	free(sep);

}

/* The following function is used to send a message (whatever message's size is) through a named pipe. */
void sendMessage(int fd,int messageSize,char* message,int bufferSize,char* sep){
	
	char messageSend[bufferSize];
	memset(messageSend,0,bufferSize);
	int tmp = 0, j;

	/* For every byte of the message. */
	while(tmp < messageSize){

		/* Fill a bufferSize's buffer using it's content. */
		for(j=0;j<bufferSize;j++){

			messageSend[j] = message[tmp++];

			if(tmp >= messageSize) break;
		}

		/* If buffer was not filled, but there is no more content to copy, break. */
		if(j < bufferSize) break;
		/* If a buffer was filled send it. */
		write(fd,messageSend,bufferSize);
	}
	
	/* If buffer was not filled, put as next byte the sep string, in my implementation '^' character, in
	order for receiver to know that we are done sending this particular message. */
	if(j < bufferSize){

		messageSend[j+1] = sep[0];
		write(fd,messageSend,bufferSize);
	/* Otherwise, if buffer was filled, send in next buffer a string that has as its first byte the character
	'^'. That will help receiver know that we are done sending bytes. */
	}else{

		memset(messageSend,0,bufferSize);
		strcpy(messageSend,sep);
		write(fd,messageSend,bufferSize);		
	}
}

/* The following function is used in order to receive a message from a named pipe. */
char* receiveMessage(int fd,int bufferSize,char sep){

	char* tempMessage = (char*)malloc(9999*sizeof(char));
	memset(tempMessage,0,9999);
	char message[bufferSize];
	memset(message,0,bufferSize);

	int tmp = 0;
	
	/* We read bufferSize messages from fd, until we get a sep character ('^'). Then we return the message that we
	have received by combining all buffers. */
	while(1){
	
		read(fd,message,bufferSize);

		for(int i=0;i<bufferSize;i++){
		
			if(message[i] == sep)
				return tempMessage;

			/* With the following case we check if parent process has terminated, in which case we should return NULL
			in order for monitor process to stop reading and wait for her to be killed. */
			if(tmp >= 9999) return NULL;
			tempMessage[tmp++] = message[i];
		}
	}
}
