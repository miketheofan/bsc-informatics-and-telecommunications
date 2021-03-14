#include "deleteFunctions.hpp"

/* The following function completely deletes a file. */
void deleteFile(string path){

	struct stat path_stat;

	if( lstat(path.c_str(),&path_stat) != 0)
		return;

	if( S_ISREG(path_stat.st_mode) ){

		if(verbose == true)
			cout << "Deleted: " << path << endl;
		remove(path.c_str());

	}else if( S_ISDIR(path_stat.st_mode) ){

		deleteDirectory(path.c_str());

	}
}

/* The following function copletely deletes a directory. */
void deleteDirectory(string file){

	DIR *pointer;
	struct dirent *d;
	struct stat s;
	string path;

	if( ( pointer = opendir(file.c_str()) ) == NULL)
		cout << "Error on opening directory to delete it." << endl;

	/* For every file in the directory, check if it is a directory and if yes recursively call the function, and if no call ::deleteFile function. */ 
	while( (d=readdir(pointer)) != NULL ){

		if(!strcmp(d->d_name,"..") || !strcmp(d->d_name,".")) continue;

		path.assign(file+"/"+string(d->d_name));
		lstat(path.c_str(),&s);

		if(S_ISDIR(s.st_mode)) deleteDirectory(path);
		else deleteFile(path);
	}

	closedir(pointer);

	if(verbose == true)
		cout << "Deleted: " << file << "/" << endl;

	/* Then copletely delete the directory herself. */
	rmdir(file.c_str());
}

/* The following function is used at the beggining of the program in order to check for deleted files in source directory, in order to delete them from
destination directory, if "-d" flag is given. */
void checkToDelete(string source,string destination){

	DIR *sDirectory , *dDirectory;
	struct dirent *sPointer , *dPointer;
	int found;
	struct stat sStat , dStat;
	string path , tempSource , tempDestination;

	if( (dDirectory = opendir(destination.c_str())) == NULL ){

		cout << "Error in destination directory in order to check for deleted files." << endl;
		closedir(dDirectory);
		return;
	}

	/* Traverse destination directory and for every file check if it exists in source directory. */
	while( (dPointer=readdir(dDirectory)) != NULL){

		found = 0;

		if( (sDirectory = opendir(source.c_str())) == NULL ){

			cout << "Error in opening source directory in order to check for deleted files" << endl;
			closedir(dDirectory);
			return;
		}

		while( (sPointer=readdir(sDirectory)) != NULL){

			if( !strcmp(sPointer->d_name,dPointer->d_name) ){

				tempSource.assign(source+"/"+string(sPointer->d_name));
				tempDestination.assign(destination+"/"+string(dPointer->d_name));

				stat(tempSource.c_str(),&sStat);
				stat(tempDestination.c_str(),&dStat);

				/* If the file exists but the one is a directory and the other a regular one, then don't do anything. */
				if( (S_ISREG(sStat.st_mode) && S_ISDIR(dStat.st_mode)) || (S_ISREG(dStat.st_mode) && S_ISDIR(sStat.st_mode)) )
					continue;

				/* Otherwise if they both are a directory, recursively call ::checkToDelete function for the two directories. */
				if( S_ISDIR(sStat.st_mode) && S_ISDIR(dStat.st_mode) )

					if( strcmp(sPointer->d_name,"..") && strcmp(sPointer->d_name,".") )
						checkToDelete(tempSource,tempDestination);

				found = 1;
				break;
			}
		}

		/* If none of the above criteria are true, then delete the file from destination directory. */
		if(!found) {
			
			path.assign(destination+"/"+string(dPointer->d_name));
			deleteFile(path);
		}

		closedir(sDirectory);
	}

	closedir(dDirectory);
	return;
}