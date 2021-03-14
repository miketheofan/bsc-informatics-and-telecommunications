#include "copyFunctions.hpp"

/* The following function gets a file's location and a directory's location as arguments, and traverses destination directory in order to check if the file exists in its' hierarchy. */
bool checkToFind(string source,string destination){

	DIR *dDirectory;
	struct dirent *dPointer;
	int found;
	struct stat Stat;
	string path , tempSource , tempDestination;

	found = 0;

	dDirectory = opendir(destination.c_str());

	while( (dPointer=readdir(dDirectory)) != NULL){

		path.assign(destination+"/"+dPointer->d_name);

		lstat(path.c_str(),&Stat);

		if( !strcmp(dPointer->d_name,"..") || !strcmp(dPointer->d_name,".") ) continue;

		if( !strcmp(source.c_str(),dPointer->d_name) ){

			tempDestination.assign(destination+"/"+dPointer->d_name);
			
			found = 1;			
			break;
		}
	}

	closedir(dDirectory);
	return found ? true : false;
}

/* The following function gets a source file's and a destination's file path as arguments, and check if files are the same according to the criteria that were given. */
bool checkCriteria(string source,string destination){

	struct stat sourceStat , destinationStat;

	stat(source.c_str(),&sourceStat);
	stat(destination.c_str(),&destinationStat);

	if( sourceStat.st_size != destinationStat.st_size )
		return false;
	if( sourceStat.st_mtime > destinationStat.st_mtime )
		return false;

	return true;
}

/* The following function gets a source directory and a destination directory as arguments, and copies all source directory's files in the destination. */
void quicFunction(string source,string destination){

	DIR *directoryPointer;
	struct dirent *direntPointer;
	struct stat Stat;
	string path , tempDestination;

	if ( ( directoryPointer = opendir( source.c_str() ) ) == NULL ){

		cout << "Error,could not find source directory." << endl;
	}else {

		/* For every file in source directory, */
		while ( ( direntPointer=readdir(directoryPointer) ) != NULL ){
		
			path = source+"/"+string(direntPointer->d_name);

			if( (!strcmp(direntPointer->d_name,"..") ) || (!strcmp(direntPointer->d_name,".")) )
				continue;

			/* "entries" variable is used to count the amount of files that exist in source directory for end-program printing reasons. */
			entries++;

			stat(path.c_str(),&Stat);
			tempDestination.assign(destination+"/"+direntPointer->d_name);

			/* If file is a directory, recursively do the same actions for every file in directory. */
			if( S_ISDIR(Stat.st_mode) ){

				if(FILE *file = fopen(tempDestination.c_str(),"r"))
					quicFunction(path,tempDestination);

			}

			/* For every file, if it isn't a directory, call function that checks if file exists in destination directory, */
			if(checkToFind(direntPointer->d_name,destination) == false){
				
				/* and if it doesn't, deep copy it there. */
				deepCopy(source,destination,string(direntPointer->d_name));
				continue;
			}else {

				/* If it exists, check the criteria that checks if files are identical, */
				if( checkCriteria(path,tempDestination) == false ){

					/* and if they aren't, deep copy it there and delete previous copy. */
					cout << "Removed: " << tempDestination << endl;
					remove(tempDestination.c_str());
					deepCopy(source,destination,string(direntPointer->d_name));
				}
			}
		}

		closedir(directoryPointer);
	}
}

/* Following function deep copies a file with name "name" , from directory "source" to directory "destination". */
void deepCopy(string source,string destination,string name){

	string sourcePath , destinationPath;
	struct stat path_stat;

	sourcePath.assign(source+"/"+name);
	destinationPath.assign(destination+"/"+name);

	if( stat(sourcePath.c_str(),&path_stat) != 0)
		return;

	/* If it is a regular file, call copy file function. */
	if( S_ISREG(path_stat.st_mode) ){

		copyFile(sourcePath,destinationPath);
		
		/* If flag "-v" was given, print that this certain file was created in destination directory. */
		if(verbose == true)
			cout << "Created: " <<  destinationPath << endl;		

	}else 
	if( name == ".." || name == "." ){

		return;

	/* If it is a directory, create the same directory in destination directory (in order to maintain the hierarchy) and copy all the files to the new directory. */
	}else if( S_ISDIR(path_stat.st_mode) ){

		/* Case that prevents infinite loops. The point is that for a directory to be copied
		it has to be created before the start of the program execution. */
		if( path_stat.st_mtime >= currentTime ) return;

		DIR *directoryPointer;
		struct dirent *direntPointer;
		mode_t bits = path_stat.st_mode;

		if(mkdir(destinationPath.c_str(),bits) == -1)
			cout << "Error creating directory." << endl;
		else{

			totalBytes+=path_stat.st_size;
			totalCopied++;
			
			if(verbose == true)
				cout << "Created: " << destinationPath << "/" << endl;
		}

		/* Open the current directory from who we want to copy the files from, */
		if ( ( directoryPointer = opendir( sourcePath.c_str() ) ) == NULL ){

			cout << "Error on opening " << sourcePath.c_str() << endl;
		}else {

			/* and copy all the files from in there in the new directory we created. */
			while ( ( direntPointer=readdir(directoryPointer) ) != NULL ){
		
				deepCopy(sourcePath,destinationPath,string(direntPointer->d_name));
			}

			closedir(directoryPointer);
		}
	/* If it is a softlink, do the same actions because we don't consider soft/hard links as different in my implementation. */
	}else if( S_ISLNK(path_stat.st_mode) ){

		copyFile(sourcePath,destinationPath);
		
		if(verbose == true)
			cout << "Created: " <<  destinationPath << endl;
	}
}

/* The following function deep copies a file given as "source" , to a file given as "destination". */
void copyFile(string source,string destination){

	FILE *fs , *fd;
	char ch;
	struct stat Stat;
	mode_t bits;

	if(stat(source.c_str(),&Stat))
		cout << "An error has occured." << endl;
 	
 	bits = Stat.st_mode;

	if( (fs = fopen(source.c_str(),"r")) == NULL )
		cout << "Error in opening file to read from." << endl;
	if( (fd = fopen(destination.c_str(),"w")) == NULL )
		cout << "Error in opening file to write to." << endl;

	/*First of all copy each character from source file to new file created. */
	ch = fgetc(fs);
	while(ch != EOF){

		fputc(ch,fd);
		ch = fgetc(fs);
	}

	/* Then copy its' perimissions, in order for the files to be indentical. */
	chmod(destination.c_str(),bits);

	totalBytes+= Stat.st_size;
	totalCopied++;

	fclose(fs);fclose(fd);

	/* Total size of the files will of course be identical, but last modification time will not be the same. We don't need it to be the same though because in ::checkCriteria we only need last modification time of 
	destination file to be after source file. */
}