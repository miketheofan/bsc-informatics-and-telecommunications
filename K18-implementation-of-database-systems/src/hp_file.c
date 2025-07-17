#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bf.h"
#include "hp_file.h"
#include "record.h"

#define CALL_BF(call)       \
{                           \
  BF_ErrorCode code = call; \
  if (code != BF_OK) {      \
    BF_PrintError(code);    \
    exit(code);             \
  }                         \
}

int HP_CreateFile(char *fileName){
    
    int fileDesc;
    char *heapID = "hp";
    char *data;
    BF_Block *block;

    /* We create a a block file using the name that was given as parameter. 
        If an error is occured, we return the corresponding error.
    */
    CALL_BF(BF_CreateFile(fileName));

    /* We open the file and store its file descriptor in a variable. 
        If an error is occured, we return the corresponding error.
    */
    CALL_BF(BF_OpenFile(fileName, &fileDesc));

    /* We initialize the block. */
    BF_Block_Init(&block);

    /* We allocate the memory needed for the file with file descriptor fileDesc.
        If an error is occured, we return the corresponding error.
    */
    CALL_BF(BF_AllocateBlock(fileDesc, block));
    
    /*
        We get the data of the block in order to insert the id type of the block, 
        to mark it as a heap file and save the number of buckets in the variable buckets.
    */
    data = BF_Block_GetData(block); 
    memcpy(data, heapID, strlen("hp")+1);

    /* We set the block dirty. */
    BF_Block_SetDirty(block);

    /* We unpin the block.
        If an error is occured, we return the corresponding error.
    */
    CALL_BF(BF_UnpinBlock(block));

    /* We close the file. */
    CALL_BF(BF_CloseFile(fileDesc));
    
    /* We destroy the block pointer. */
    BF_Block_Destroy(&block);
    return HP_OK;
}

HP_info* HP_OpenFile(char *fileName){
    
    char *data;
    char testHeap[strlen("hp")+1];
    BF_Block *block;
    int fileDesc;
    int blocksNum;
    int blockEntries;
    
    /* We open the file and store its file descriptor in a variable. 
        If an error is occured, we return the corresponding error.
    */
    BF_OpenFile(fileName, &fileDesc);
        
    HP_info* info = malloc(sizeof(HP_info));

    /* We store the file descriptor of the block inside the info block
    that is to be returned. */
    info->fileDesc = fileDesc;

    /* We get the total amount of blocks inside the block with file
    descriptor equal to fileDesc. */
    BF_GetBlockCounter(fileDesc, &blocksNum);

    /* We initialize the block. */
    BF_Block_Init(&block);  

    /* We get the block with number 0 in the file with file descriptor
    fileDesc, and return it.
        If an error is occured, we return the corresponding error.
     */
    CALL_BF(BF_GetBlock(fileDesc, 0, block));

    /* We get the data of the block. */
    data = BF_Block_GetData(block); 

    /* We ensure that it is a hash file and get the number of buckets
    that it contains. */
    memcpy(testHeap, data, strlen("hp")+1);
    if(strcmp(testHeap,"hp") != 0){
        printf("The given file is not a Heap File.\n");
        return NULL;
    }

    /* We read the number of entries inside the block. */
    memcpy(&blockEntries, data, sizeof(int)); 

    /* We unpin the block.
        If an error is occured, we return the corresponding error.
    */
    BF_UnpinBlock(block);
    
    /* We destroy the block pointer. */
    BF_Block_Destroy(&block);
    return info;
}


int HP_CloseFile( HP_info* hp_info ){
        
    /* We close the proper block file. 
        If an error is occured, we return the corresponding error.
    */
    CALL_BF(BF_CloseFile(hp_info->fileDesc));
    
    return HP_OK;
}

int HP_InsertEntry(HP_info* hp_info, Record record){
    
    int offset;
    char *data;
    BF_Block *block;
    int fileDesc = hp_info->fileDesc;
    int blocksNum;
    int blockEntries;
    
    /* We get the total amount of blocks inside the block with file
    descriptor equal to fileDesc. */
    CALL_BF(BF_GetBlockCounter(fileDesc, &blocksNum));

    /* We initialize the block. */
    BF_Block_Init(&block);

    /* We get the block with number blocksNum - 1 in the file with file descriptor
    fileDesc, and return it.
        If an error is occured, we return the corresponding error.
     */
    CALL_BF(BF_GetBlock(fileDesc, blocksNum - 1, block));

    /* We fetch the block's data. */
    data = BF_Block_GetData(block);

    /* We read the number of entries inside the block. */
    memcpy(&blockEntries, data, sizeof(int));

    /* If we are inside the first block or we have reached the cap of the blocks. */
    if (blocksNum == 1 || blockEntries == BLOCK_CAP){ 

        /* We unpin the block. */
        CALL_BF(BF_UnpinBlock(block));
        
        /* We allocate a new one. */
        CALL_BF(BF_AllocateBlock(fileDesc, block));
                
        /* We get the data of the block. */
        data = BF_Block_GetData(block);
        blockEntries = 0;
    }

    /* We add the new entry.
        We add the name, the surname and the address.
     */
    offset = sizeof(int) + blockEntries * sizeof(Record);
    memcpy(data + offset, &record.id, sizeof(int));
    offset = offset + sizeof(int);
    memcpy(data + offset, record.name, 15);
    offset = offset + 15;
    memcpy(data + offset, record.surname, 20);
    offset = offset + 20;
    memcpy(data + offset, record.address, 50);
    blockEntries++;
    memcpy(data, &blockEntries, sizeof(int));

    /* We set the block dirty. */
    BF_Block_SetDirty(block);

    /* We unpin the block. */
    CALL_BF(BF_UnpinBlock(block));
    
    /* We destroy the block. */
    BF_Block_Destroy(&block);
    return HP_OK;
}

int HP_GetAllEntries(HP_info* hp_info, int value){
   
    int fileDesc = hp_info->fileDesc;
    int blockEntries;
    int offset;
    char *data;
    Record record;
    BF_Block *block;
    int blocksNum;
    
    /* We get the total amounts of entries inside the block. */
    CALL_BF(BF_GetBlockCounter(fileDesc, &blocksNum));

    /* We initialize the block. */
    BF_Block_Init(&block);

    /* We traverse all the blocks. */
    for(int i=1 ; i<blocksNum ; i++){
        
        /* We get the block. 
            If an error is occured, we return the corresponding error.
        */
        CALL_BF(BF_GetBlock(fileDesc, i, block));
        
        /* We get the data of the block. */
        data = BF_Block_GetData(block);
        memcpy(&blockEntries, data, sizeof(int)); 
        
        /* We traverse all entries inside the block. */
        for(int j=0 ; j<blockEntries ; j++){
            
            offset = sizeof(int) + j * sizeof(Record);
            memcpy(&record, data + offset, sizeof(Record)); //Used for printing.
            
            /* If we need to print all entries, then we print it. */
            if(value == PRINT_ALL){
                printf("Id: %d, Name: %s, Surname: %s, City: %s\n", record.id, record.name, record.surname, record.address);
            }
            /* Otherwise, check if record's id is equal to the value and only then 
                print it. */
            else{
                if(record.id == value) {
                    printf("Id: %d, Name: %s, Surname: %s, City: %s\n", record.id, record.name, record.surname, record.address);
                }
            }
        }
        
        /* We unpin the block. */
        CALL_BF(BF_UnpinBlock(block));
    }
    
    /* We destroy the block. */
    BF_Block_Destroy(&block);
    return HP_OK;
}

