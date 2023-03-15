#include "../include/Dictionary.h"

/* The following function initializes a dictionary node. */
void initialize(Dictionary* dict){

	dict->name = NULL;
	dict->x = -1;
	dict->y = -1;
	dict->next = NULL;
}

/* The following function inserts a new node in the dictionary list. */
void insert(Dictionary* dict,char* name,int x,int y){

	/* If this current node is empty, then insert the data given as parameter. */
	if(dict->name == NULL){		
		dict->x = x;
		dict->y = y;
		dict->name = malloc(20);
		strcpy(dict->name,name);
	/* Otherwise */
	}else{
		/* If there is not a next node allocated, allocate it and insert the 
			data in the next node. */
		if(dict->next == NULL){
			dict->next = (Dictionary*)malloc(sizeof(Dictionary));
			initialize(dict->next);
			insert(dict->next,name,x,y);
		/* If there is a next node allocated, then insert the data in the next node. */
		}else{
			insert(dict->next,name,x,y);
		}
	}

	return;
}

/* The following function checks whether a name with given coordinates
	exists in the dictionary or not. */
bool find(Dictionary* dict,char* name,int x,int y){

	// printf("Searching: %s, be at %s\n",name,dict->name);

	if(dict->x == x && dict->y == y){
		if(!strcmp(dict->name,name)){
			return true;
		}else{
			return false;
		}
	}else{
		if(dict->next != NULL){
			return find(dict->next,name,x,y);
		}else{
			return false;
		}
	}
}