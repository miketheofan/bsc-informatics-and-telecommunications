#include "quic.hpp"

int main(int argc,char **argv){

	clock_t tStart = clock();
	double executionTime;
	DIR *checkIfDestExists ,*checkIfSourceExists;

	currentTime = time(0);

	string origindir, destdir;

	if(argc < expectedArgc){

		cout << "Wrong input.Exiting program." << endl;
		exit(1);
	}

	/* Check the flags that were given with the execution of the program. */
	for(int i=0;i<argc;i++)

		if(!strcmp(argv[i],"-v")) verbose = true;
		else if(!strcmp(argv[i],"-d")) deleted = true;

	/* According to the flags given, initialize the rest argv entries, as source and destination libraries location. */ 
	if( verbose == true && deleted == true ){

		origindir = argv[3];
		destdir = argv[4];
	}else if( verbose == true || deleted == true ){

		origindir = argv[2];
		destdir = argv[3];
	}else{

		origindir = argv[1];
		destdir = argv[2];
	}

	/* Check if path that was given as source directory's location exists and if not, return. */
	if( (checkIfSourceExists = opendir(origindir.c_str())) == NULL ){

		cout << "Error on opening source directory.Returning." << endl;
		closedir(checkIfSourceExists);
		return 0;
	}

	/* Check if path that was given as destination directory's location exists and if not, create it. */
	if( (checkIfDestExists = opendir(destdir.c_str())) == NULL){

		if(mkdir(destdir.c_str(),defaultPermissions) == -1){

			cout << "Error on creating destination directory." << endl;
			closedir(checkIfDestExists);
			return 0;
		}else{

			cout << "created directory " << destdir << endl;
		}
	}

	/* If flag "-d" was given, call the function that deletes files/directories that were deleted by source directory. */
	if(deleted == true)
		checkToDelete(origindir,destdir);
	
	quicFunction(origindir,destdir);

	cout << "there are " << entries << " files/directories in the hierarchy" << endl; 

	executionTime = (double)(clock() - tStart)/CLOCKS_PER_SEC;

	cout << "number of entities copied is " << totalCopied << endl;
	cout << "copied " << setprecision(2) << showpoint << fixed << totalBytes << " bytes in " << executionTime 
	<< " at " << setprecision(2) << showpoint << fixed << totalBytes/executionTime << " bytes/sec" << endl;

	closedir(checkIfDestExists);
	closedir(checkIfSourceExists);

	return 0;
}