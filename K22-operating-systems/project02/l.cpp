#include "l.hpp"

int prime1(int);
int prime2(int);
int prime3(int);
void signalFunction(int);

int main(int argc,char** argv){

/*Same process as myprimes.cpp except from the fact that we use different arguments for W nodes
 and except from reading from pipes, it also writes to pipe given as argument*/
	int lb,ub,children,fdParent,assign,parentid;
	
	if(argc != expectedArgsi){

		cout << "Wrong input.Exiting L" << endl;
		return 0;
	}

	for(int i=0;i<argc;i++)

		if(!strcmp(argv[i],"-l"))
			lb = stoi(argv[++i]);
		else if(!strcmp(argv[i],"-u"))
			ub = stoi(argv[++i]);
		else if(!strcmp(argv[i],"-w"))
			children = stoi(argv[++i]);
		else if(!strcmp(argv[i],"-p"))
			fdParent = stoi(argv[++i]);
		else if(!strcmp(argv[i],"-a"))
			assign = stoi(argv[++i]);
		else if(!strcmp(argv[i],"-i"))
			parentid = stoi(argv[++i]);

	int tempLowest = lb , **fdChild = new int*[children];
	pid_t pid;
	char** vector = (char**)malloc(sizeof(char*)*(expectedArgsw+1));
	string listEntry,string1 = to_string(1), string2 = to_string(2), string3 = to_string(3) ,
	temp_lb , temp_ub , temp_children , temp_fd , temp_id;

	for(int i=0;i<expectedArgsw;i++)
		vector[i] = (char*)malloc(defaultArgsSize);
	vector[expectedArgsw] = NULL;

	for(int i=0;i<children;i++){

		fdChild[i] = new int[2];
		pipe(fdChild[i]);
	}

	for(int i=0;i<children;i++){

		if(assign == numberOfFunctions+1) assign = 1;

		temp_lb = to_string(tempLowest);
		temp_children = to_string(children);
		temp_id = to_string(parentid);
		if(i != children-1)
			temp_ub = to_string(tempLowest + (ub-lb)/children );
		else
			temp_ub = to_string(ub);

		strcpy(vector[0],"./l");
		strcpy(vector[1],"-l");//lb
		strcpy(vector[2],temp_lb.c_str());
		strcpy(vector[3],"-u");//ub
		strcpy(vector[4],temp_ub.c_str());
		strcpy(vector[5],"-i");//parent id
		strcpy(vector[6],temp_id.c_str());
		strcpy(vector[7],"-f");//primes function 

		
		switch(assign++){

			case(1):{

				strcpy(vector[8],string1.c_str());
				break;
			}case(2):{

				strcpy(vector[8],string2.c_str());
				break;
			}case(3):{

				strcpy(vector[8],string3.c_str());
				break;
			}
		}
		
		pid = fork();

		if(pid == 0) {
			
			close(*(fdChild[i]+0));

			temp_fd = to_string(*(fdChild[i]+1));

			strcpy(vector[9],"-p");//pipe that worker will use
			strcpy(vector[10],temp_fd.c_str());

			if(execvp("./w",vector) == -1)
				cout << "Error creating W process." << endl;
		}
		tempLowest = tempLowest + (ub-lb)/children;
	}

	int val,checkError;
	double valTime,totalTime;
	List *list = new List;

/*It inserts all data received by workers on a list*/
	for(int i=0;i<children;i++){

		close(*(fdChild[i]+1));

		while( ( checkError = read(*(fdChild[i]+0),&valTime,sizeof(valTime)) ) > 0){

			if( read(*(fdChild[i]+0),&val,sizeof(val)) > 0)
				list->insert(val,valTime);
			else 
				list->insert(totalTimeValue,totalTime = valTime);
		}

		close(*(fdChild[i]+0));
	}

	int j=0 ,size = list->getSize();

	if(write(fdParent,&size,sizeof(size)) == -1)
		cout << "Error writting in pipe." << endl;


/*And then inserts all list data to a static array, which array will then send to parent node.*/
	List * listPointer = list;
	double batch[size+1][2];

	for(int i=0;i<size+1;i++){

		batch[i][1] = -1;
		batch[i][2] = -1;
	}

	while(listPointer != NULL){

		batch[j][0] = listPointer->getNumber();
		batch[j++][1] = listPointer->getTime();

		listPointer = listPointer->getNext();
	}

	batch[size][1] = totalTime;

	if(write(fdParent,&batch,sizeof(batch)) == -1)
		cout << "Error writting in pipe." << endl;

	close(fdParent);

	for(int i=0;i<expectedArgsw+1;i++)
		free(vector[i]);	
	free(vector);

	for(int i=0;i<children;i++)
		delete[] fdChild[i];
	delete[] fdChild;

	delete list;
}