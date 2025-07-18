#include "SkipList.hpp"

SkipListHeader::SkipListHeader(int maximumLevel, int currentLevel , float probability) : maximumLevel(maximumLevel) , currentLevel(currentLevel) , probability(probability) {

	SkipListHeader *nextNode;

	this->nextLayer = NULL;
	this->previousLayer = NULL;
	this->list = NULL;

	if(currentLevel < maximumLevel){

		nextNode = new SkipListHeader(maximumLevel,this->currentLevel+1,probability);
		this->nextLayer = nextNode;
		nextNode->previousLayer = this;
	}
}

bool SkipListHeader::isEmpty(){
	return this->list == NULL ? true : false;
}

void SkipListHeader::print(){

	if( this->nextLayer != NULL )
		this->nextLayer->print();

	cout << "Layer: " << this->currentLevel << endl;

	if(this->list != NULL){

		this->list->print();
		cout << endl;
	}
}

void SkipListHeader::printCitizens(){
	this->list->printCitizens();
}

/* The following function generates a random number using the probability given as a parameter
in the initialization of the skip list. This number is used to indicate till which level a node will
be extended when being inserted in the list. */
int SkipListHeader::randomLevelGenerator(){ 

    double randomNumber; 
    int count = -1; 

    do{

    	if(++count > this->maximumLevel-1) break;

    	randomNumber = (double)rand()/RAND_MAX; 

    }while(randomNumber < this->probability);

    return count;
}; 

/* The following function inserts a node in the skip list using the algorithm shown in the source given
by the instructors: https://www.youtube.com/watch?v=UGaOXaXAM5M . */
void SkipListHeader::insert(Citizen* citizen){

	SkipList *newNode, *update[maximumLevel+1][2], *tempNode, *connect[maximumLevel+1];
	int key = citizen->getID();
	
	/* We use the update[][] array in order to "store" the previous and next node of the new node, for every
	height om which he might be extended. Of course we probably won't need the info for all the levels, but
	just to be safe from the beggining we store every info there. */
	for(int i=0;i<maximumLevel+1;i++){

		connect[i] = NULL;

		for(int j=0;j<2;j++)
			update[i][j] = NULL;
	}

	SkipListHeader *tempHeader = this;

	/* First of all, traverse to the last level of the skip list (top layer). */
	while(tempHeader->nextLayer != NULL)
		tempHeader = tempHeader->nextLayer;

	/* Till we reach the bottom level: */
	while(tempHeader != NULL){

		/* If currentLayer is empty, skip it. */
		if(tempHeader->list == NULL){

			tempHeader = tempHeader->previousLayer;
			continue;
		}

		tempNode = tempHeader->list;
		update[tempHeader->currentLevel][1] = tempNode;

		/* For every layer, traverse the list in order to find the spot where the new
		node must be placed. For every spot, store the previous and next node in the update array. */
		while(tempNode != NULL && tempNode->citizen->getID() < key) {

			update[tempHeader->currentLevel][0] = tempNode;
			update[tempHeader->currentLevel][1] = tempNode->nextNode;			
			tempNode = tempNode->nextNode;
		}

		/* Continue to the previous layer, so that we will eventually reach the bottom layer. */
		tempHeader = tempHeader->previousLayer;
	}

	/* Generate a random level. */
	int randomLayer = randomLevelGenerator();

	tempHeader = this;

	/* For every layer starting from the bottom till we reach the random layer generated: */
	for(int i=0; i<randomLayer+1; i++){

		/* Create the new node. */
		newNode = new SkipList(i,citizen);

		/* We use the connect[] array in order to connect the new nodes from every layer together.
		In other words, we need to have Layer 0 newNode connected to Layer 1 newNode , etc in order to 
		continue with our implementation. */
		if(i>0) newNode->previousLayer = connect[i-1];	
		connect[i] = newNode;
		
		/* If current layer is empty, insert new node as the first-and only node of layer. */
		if(tempHeader->list == NULL){

			tempHeader->list = newNode;
			tempHeader = tempHeader->nextLayer;
			connect[i] = newNode;
			continue;
		}

		/* If previous node is empty, then new node will be the first node of layer. */
		if(update[i][0] == NULL){

			tempHeader->list = newNode;
			update[i][1]->previousNode = newNode;
		}

		/* If next node = NULL, then new node will be the last node of layer. */
		if(update[i][1] == NULL)
			update[i][0]->nextNode = newNode;

		/* Otherwise, insert new node to the list by switching previousNode's and nextNode's pointers. */
		if(update[i][1] != NULL && update[i][0] != NULL){

			update[i][0]->nextNode = newNode;
			update[i][1]->previousNode = newNode;
		}

		newNode->previousNode = update[i][0];
		newNode->nextNode = update[i][1];

		/* Move to next Layer. */
		tempHeader = tempHeader->nextLayer;
	}

	for(int i=0;i<maximumLevel+1;i++){

		connect[i] = NULL;

		for(int j=0;j<2;j++)
			update[i][j] = NULL;
	}
}

/* The following function searches for an id in skip list. My implementation follows the instructions shown in the
ource given by the instructors: https://www.youtube.com/watch?v=UGaOXaXAM5M . */ 
Citizen* SkipListHeader::search(int key){

	SkipListHeader *tempHeader = this;
	SkipList *tempNode, *previousNode;

	/* First of all, traverse to the last layer of skip list. */
	while(tempHeader->nextLayer != NULL)
		tempHeader = tempHeader->nextLayer;

	tempNode = tempHeader->list;
	previousNode = NULL;

	/* For every layer till we reach bottom layer: */
	while(tempHeader != NULL){

		/* Traverse current's layer list till we find the id, or find an id that is bigger than key we are searching for. */
		while(tempNode != NULL && tempNode->citizen->getID() <= key){

			if(tempNode->citizen->getID() == key)
				return tempNode->citizen;

			previousNode = tempNode;
			tempNode = tempNode->nextNode;
		}

		/* Continue to previous layer. */
		tempHeader = tempHeader->previousLayer;	
	
		/* If we reached the end of the list , continue to previous layer. */
		if(tempHeader == NULL) continue;

		/* If we didn't traverse any node of the list (because of tempNode->content > key)
		then continue to previousLayer start of the list. */
		if(previousNode == NULL){

			tempNode = tempHeader->list;
			continue;
		}

		/* Or just continue to previous Node's previous Layer. For example:
			4 -> 7 -> 9
		If we were searching for 8, previousNode would be 7 and we would continue to previousLayer's 7 node. */
		tempNode = previousNode->previousLayer;
	}

	return NULL;
}
	
/* The following function deletes a given node from skip list. My implementation follows the instructions shown in the
ource given by the instructors: https://www.youtube.com/watch?v=UGaOXaXAM5M . */ 
void SkipListHeader::Delete(int key){

	/* We again use a 2D array in order to know which is the previousNode,nextNode and of course the node we have to delete.
	For each layer[i]: [i][0] is previousNode, [i][1] is nodeToDelete , [i][2] is nextNode. */
	SkipList *deleteBetween[maximumLevel+1][3], *tempNode;
	
	for(int i=0;i<maximumLevel+1;i++)
		for(int j=0;j<3;j++)
			deleteBetween[i][j] = NULL;

	SkipListHeader *tempHeader = this;

	/* Again, we traverse to the last layer. */
	while(tempHeader->nextLayer != NULL)
		tempHeader = tempHeader->nextLayer;

	/* Till we reach bottom layer: */
	while(tempHeader != NULL){

		if(tempHeader->list == NULL){

			tempHeader = tempHeader->previousLayer;
			continue;
		}

		tempNode = tempHeader->list;

		/* Traverse each node of the list, till we find node that we have to delete, and then and
		only then update the array. */
		while(tempNode != NULL && tempNode->citizen->getID() <= key) {
	
			if(tempNode->citizen->getID() == key){

				deleteBetween[tempHeader->currentLevel][0] = tempNode->previousNode;
				deleteBetween[tempHeader->currentLevel][1] = tempNode;
				deleteBetween[tempHeader->currentLevel][2] = tempNode->nextNode;
				break;
			}

			tempNode = tempNode->nextNode;
		}

		tempHeader = tempHeader->previousLayer;
	}

	tempHeader = this;

	/* For every layer starting from the bottom one till the last layer: */
	for(int i=0;i< this->maximumLevel+1;i++){

		if(deleteBetween[i][1] != NULL){ /* If node we have to delete is found */

			if(deleteBetween[i][0] == NULL && deleteBetween[i][2] == NULL) /* If it is the only node in the list, then list gets empty. */
				tempHeader->list = NULL;
			else if(deleteBetween[i][0] == NULL && deleteBetween[i][2] != NULL){ /* If node is the first node of the list, then list's first node
															will be nextNode(deleteBetween[i][2]) . */

				deleteBetween[i][1]->nextNode = NULL;
				tempHeader->list = deleteBetween[i][2];
			}else if(deleteBetween[i][0] != NULL && deleteBetween[i][2] == NULL) /* If node is last node of the list, then list's last node
															will be previousNode(deleteBetween[i][0]) . */
				deleteBetween[i][0]->nextNode = NULL;
			else if(deleteBetween[i][0] != NULL && deleteBetween[i][2] != NULL){ /* Otherwise just remove node from the list by updating the pointers. */

				deleteBetween[i][0]->nextNode = deleteBetween[i][2];
				deleteBetween[i][2]->previousNode = deleteBetween[i][0];
				deleteBetween[i][1]->nextNode = NULL;
			}

			delete deleteBetween[i][1]; /* And then delete the pointer to node. */
		}

		/* Traverse to nextLayer. */
		tempHeader = tempHeader->nextLayer;
	}
}

SkipListHeader::~SkipListHeader(){

	if(this->list != NULL) delete this->list;
	if(this->nextLayer != NULL) delete this->nextLayer;
}

SkipList::SkipList(int currentLevel , Citizen* citizen) : currentLevel(currentLevel){

	this->nextNode = NULL;
	this->previousNode = NULL;
	this->previousLayer = NULL;
	this->citizen = citizen;
}

void SkipList::print(){

	cout << this->citizen->getID() << " ";
	
	if(this->nextNode != NULL)
		this->nextNode->print();
}

void SkipList::printCitizens(){

	cout << this->citizen->getID() << ", " << this->citizen->getFirstName() << ", " << this->citizen->getLastName()
	<< ", " << this->citizen->getCountry() << ", " << this->citizen->getAge() << endl; 

	if(this->nextNode != NULL) this->nextNode->printCitizens();
}

SkipList::~SkipList(){
	if(this->nextNode != NULL) delete this->nextNode;
}