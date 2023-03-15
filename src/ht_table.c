#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "bf.h"
#include "ht_table.h"
#include "record.h"

#define CALL_OR_DIE(call)     \
  {                           \
    BF_ErrorCode code = call; \
    if (code != BF_OK) {      \
      BF_PrintError(code);    \
      exit(code);             \
    }                         \
  }

int hashFunction(int id, int buckets){
  return id % buckets;
}

/* The following function calculates the total amount of blocks required, given
the number of buckets and using the total buffer size. */
int calculateBlocksNumber(int buckets){
  return buckets * (BF_BUFFER_SIZE / buckets);
}

/* The following function returns the position of memory inside a block, 
given the hash and the number of buckets. */
int getBlockPosition(int hash,int buckets){
  return hash * (BF_BUFFER_SIZE / (BF_BUFFER_SIZE / buckets));
}

int HT_CreateFile(char *fileName, int buckets){
  
  int fileDesc;

  /* We first create the file with the file name given as a parameter. */
  CALL_OR_DIE(BF_CreateFile(fileName));

  /* We then open the file and store its' file descriptor. */
  CALL_OR_DIE(BF_OpenFile(fileName, &fileDesc));

  BF_Block *block;

  /* We initiaze the first block that we will need to store our data. */
  BF_Block_Init(&block);

  /* We allocate the block using the file descriptor. */
  CALL_OR_DIE(BF_AllocateBlock(fileDesc, block));

  /* We get the data of the block. */
  char *data = BF_Block_GetData(block);

  /* We calculate the total amount of blocks that we are goind to need for this run. */
  int blocks = calculateBlocksNumber(buckets);

  /* We insert the information needed inside the block's data. */
  HT_block_info ht_block;
  strcpy(ht_block.id,"ht");
  ht_block.buckets = buckets;

  /* We store the ht block inside the memory. */
  memcpy(data, &ht_block, sizeof(HT_block_info));

  /* We set the block dirty. */
  BF_Block_SetDirty(block);

  /* We unpin the block. */
  CALL_OR_DIE(BF_UnpinBlock(block));

  /* We allocate the blocks we need in order to insert the data inside. */
  for (int i = 0; i < blocks; i++){
    CALL_OR_DIE(BF_AllocateBlock(fileDesc, block));
    CALL_OR_DIE(BF_UnpinBlock(block));
  }
  
  /* We destroy the block. */
  BF_Block_Destroy(&block);

  /* We close the file. */
  CALL_OR_DIE(BF_CloseFile(fileDesc));

  return HT_OK;
}

HT_info *HT_OpenFile(char *fileName){

  int fileDesc;

  CALL_OR_DIE(BF_OpenFile(fileName, &fileDesc));

  /* We initialize the block. */
  BF_Block *block;
  BF_Block_Init(&block);

  /* We get the block in the first position. */
  CALL_OR_DIE(BF_GetBlock(fileDesc, 0, block));

  /* We get the data of the block. */
  char *data;
  data = BF_Block_GetData(block);

  /* We store the data inside the memory in the block variable. */
  HT_block_info blockInfo;
  memcpy(&blockInfo, data, sizeof(HT_block_info));


  /* We check if the info block is of type ht and if not, we close it. */
  if (strcmp(blockInfo.id,"ht")){
    /* We unpin the block. */
    CALL_OR_DIE(BF_UnpinBlock(block));
    /* We destory the block. */
    BF_Block_Destroy(&block);
    /* We close the file. */
    CALL_OR_DIE(BF_CloseFile(fileDesc));
    /* We return null to indicate an error. */
    return NULL;
  }

  /* We initialize the info block. */
  HT_info *info = malloc(sizeof(HT_info));
  info->filename = fileName;
  info->fileDesc = fileDesc;
  info->buckets = blockInfo.buckets;
  info->lastRecord = (BF_BLOCK_SIZE) / sizeof(Record);

  /* We unpin the block. */
  CALL_OR_DIE(BF_UnpinBlock(block));

  /* We destroy the block. */
  BF_Block_Destroy(&block);

  /* We return the info block. */
  return info;
}

int HT_CloseFile(HT_info *header_info){

  /* We close the file using the file descriptor of the info block given as input. */
  CALL_OR_DIE(BF_CloseFile(header_info->fileDesc));
  return HT_OK;
}

int HT_InsertEntry(HT_info *header_info, Record record){

  /* We get the hash position of the record's id given. */
  int hash = hashFunction(record.id, header_info->buckets);

  /* We get the position of the block that it needs to be inserted at. */
  int blockPosition = getBlockPosition(hash,header_info->buckets);

  /* We initialize the block that we are going to insert with the record. */
  BF_Block *blockNew;  
  BF_Block_Init(&blockNew);

  /* We initialize the block where we need to insert the record at. */
  BF_Block *blockOld;  
  BF_Block_Init(&blockOld);

  CALL_OR_DIE(BF_GetBlock(header_info->fileDesc, 0, blockOld));

  /* We get the data of the block that we need for the insertion. */
  char *dataOld;
  dataOld = BF_Block_GetData(blockOld);

  /* We calculate the block position where the new data must be inserted. */
  int *posPointer = (int *)(dataOld + sizeof(HT_block_info) + blockPosition * sizeof(int));
  int pos = posPointer[0];

  /* We check if we have no entries yet, in which case we start from the 
  first block. */
  blockPosition == 0 ? blockPosition++ : blockPosition;

  /* We traverse the blocks to find the position until we have reached the final record. */
  while (pos >= header_info->lastRecord){
    blockPosition++;
    memcpy(&pos, dataOld + 8 + blockPosition * 4, sizeof(int));
  }

  /* If we have not reached the last record. */
  if (pos < header_info->lastRecord){
    /* We get the block given the position that we have to insert our new block. */
    CALL_OR_DIE(BF_GetBlock(header_info->fileDesc, blockPosition, blockNew));

    /* We get the data of the new block. */
    char *dataNew = BF_Block_GetData(blockNew);

    /* We insert the data in the block. */
    memcpy(dataNew + pos * sizeof(Record), &record, sizeof(Record));

    /* We set the block dirty. */
    BF_Block_SetDirty(blockNew);

    /* We unpin the block. */
    CALL_OR_DIE(BF_UnpinBlock(blockNew));

    /* We destroy the block. */
    BF_Block_Destroy(&blockNew);
  /* Otherwise we have reached the end of the block and we return error. */
  }

  /* We initialize the new position of the block in order to insert it inside the
  old block. */
  pos++;
  memcpy(dataOld + 8 + blockPosition * 4, &pos, 4);

  /* We set the block dirty. */
  BF_Block_SetDirty(blockOld);

  /* We destroy the block. */
  BF_Block_Destroy(&blockOld);
  
  /* We return the position of the block where the record was inserted. */
  return blockPosition;
}

int HT_GetAllEntries(HT_info *header_info, int *value) {

  /* We initialize the block that we are going to insert with the record. */
  BF_Block *blockNew;
  BF_Block_Init(&blockNew);

  /* We initialize the block where we need to insert the record at. */
  BF_Block *blockOld;
  BF_Block_Init(&blockOld);

  /* We get the data of the block that we need for the insertion. */
  char *dataOld;

  /* We get the first block of the file with the right file descriptor. */
  CALL_OR_DIE(BF_GetBlock(header_info->fileDesc, 0, blockOld));

  /* We get the data of the block. */
  dataOld = BF_Block_GetData(blockOld);

  /* We get the total amount of blocks that exist inside the file with the file descriptor
  given inside the header_info. */
  int blocks;
  CALL_OR_DIE(BF_GetBlockCounter(header_info->fileDesc, &blocks));

  int recordsTraversed = 0;

  /* If value is NULL, then we need to print all the records inside the block. */
  if (value == NULL){
    /* We traverse all blocks. */
    for(int i = 1;i < blocks;i++){

      /* We calculate the block position for the current block. */
      int *posPointer = (int *)(dataOld + sizeof(HT_block_info) + i * sizeof(int));
      int pos = posPointer[0];
      
      /* We get the block. */
      BF_GetBlock(header_info->fileDesc, i, blockNew);
      
      /* We get the data of the block. */
      char *data = BF_Block_GetData(blockNew);
      
      /* We traverse all records inside the block. */
      for (int j = 0; j < pos; j++){
        Record record;
        /* We get the record data inside the block. */
        memcpy(&record, data + j * sizeof(Record), sizeof(Record));
        /* We print the record. */
        printRecord(record);
        /* We update the total amount of records that we traversed,
        in order for it to be returned at the end of execution. */
        recordsTraversed++;
      }

      /* We unpin the block. */
      CALL_OR_DIE(BF_UnpinBlock(blockNew));
    }
  /* Otherwise we print all records that have id equal to the id of the record given
  as a parameter. */
  }else{

    /* We get the hash position of the record's id given. */
    int hashValue = hashFunction(*value, header_info->buckets);

    /* We get the position of the block that we need to search at. */
    int blockPosition = getBlockPosition(hashValue,header_info->buckets);

    /* We check if we are at the block with position equal to zero. If yes, we go to the
    first block. */
    blockPosition == 0 ? blockPosition++ : blockPosition;

    /* We traverse all blocks. */
    for(int i = blockPosition;i < blocks;i++){

      /* We calculate the block position for the current block. */
      int *posPointer = (int *)(dataOld + sizeof(HT_block_info) + i * sizeof(int));
      int pos = posPointer[0];

      /* We get the block in this position. */
      BF_GetBlock(header_info->fileDesc, i, blockNew);

      /* We get the data inside the block. */
      char *dataNew = BF_Block_GetData(blockNew);
      
      /* We traverse all records inside the block. */
      for (int j = 0; j < pos; j++){

        /* We get the record data inside the block. */
        Record rec;
        memcpy(&rec, dataNew + j * sizeof(Record), sizeof(Record));

        /* We update the total amount of records that we traversed,
        in order for it to be returned at the end of execution. */
        recordsTraversed++;

        /* If the record's id is equal to the value given as a parameter */
        if(rec.id == *value){
          /* Then we print the record. */
          printRecord(rec);
        }
      }

      /* We unpin the block. */
      CALL_OR_DIE(BF_UnpinBlock(blockNew));      
    }
  }

  /* We unpin the entire block. */
  CALL_OR_DIE(BF_UnpinBlock(blockOld))

  /* We destroy the interior block. */
  BF_Block_Destroy(&blockNew);

  /* We also destroy the entire block. */
  BF_Block_Destroy(&blockOld);

  /* We return the total amount of records that were traversed. */
  return recordsTraversed;
}