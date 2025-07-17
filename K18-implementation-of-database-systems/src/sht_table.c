#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/bf.h"
#include "../include/sht_table.h"
#include "../include/ht_table.h"
#include "../include/record.h"

#define CALL_OR_DIE(call)     \
  {                           \
    BF_ErrorCode code = call; \
    if (code != BF_OK) {      \
      BF_PrintError(code);    \
      exit(code);             \
    }                         \
  }

int SHT_CreateSecondaryIndex(char *sfileName, int buckets, char *fileName) {
  
  int fileDesc;

  /* We create the file using the file name given as a parameter. */
  CALL_OR_DIE(BF_CreateFile(sfileName));

  /* We open the file and store its' file descriptor in the corresponding variable. */
  CALL_OR_DIE(BF_OpenFile(sfileName, &fileDesc));

  /* We initialize the block. */
  BF_Block *block;
  BF_Block_Init(&block);

  /* We allocate the block using the file descriptor that we have already initialized. */
  CALL_OR_DIE(BF_AllocateBlock(fileDesc, block));

  /* We get the data from the block. */
  char *data = BF_Block_GetData(block);
 
  /* We calculate the total amount of blocks that we are goind to need for this run. */
  int blocks = calculateBlocksNumber(buckets);

  /* We initialize an info block. */
  SHT_block_info info;

  strcpy(info.id,"sht");
  info.buckets = buckets;

  /* We insert the data inside the info block. */
  memcpy(data, &info, sizeof(SHT_block_info));

  /* We set the block dirty. */
  BF_Block_SetDirty(block);

  /* We unpin the block. */
  CALL_OR_DIE(BF_UnpinBlock(block));

  /* We allocate the block. */
  CALL_OR_DIE(BF_AllocateBlock(fileDesc, block));

  /* We unpin the block. */
  CALL_OR_DIE(BF_UnpinBlock(block));

  /* We allocate all the blocks that we are goind to need for our
  functionality. */
  for (int i = 0; i < blocks; i++){
    /* We allocate each block. */
    CALL_OR_DIE(BF_AllocateBlock(fileDesc, block));
    /* We unpin each block. */
    CALL_OR_DIE(BF_UnpinBlock(block));
  }

  /* We destroy the block. */
  BF_Block_Destroy(&block);

  /* We close the file. */
  CALL_OR_DIE(BF_CloseFile(fileDesc));

  return SHT_OK;
}

SHT_info *SHT_OpenSecondaryIndex(char *sfileName){

  int fileDesc;
  int blocks;

  /* We open the file. */
  CALL_OR_DIE(BF_OpenFile(sfileName, &fileDesc));

  /* We get the total amount of blocks inside the file. */
  BF_GetBlockCounter(fileDesc, &blocks);

  /* We initialize the block. */
  BF_Block *block;
  BF_Block_Init(&block);

  /* We get the first block of the file with file descriptor given. */
  CALL_OR_DIE(BF_GetBlock(fileDesc, 0, block));

  /* We get the data of the block. */
  char *data;
  data = BF_Block_GetData(block);

  /* We initialize an info block, and store the data that we got from the block to it. */
  SHT_block_info ht;
  memcpy(&ht, data, sizeof(HT_block_info));

  /* We check if the info block is of type sht and if not, we close it. */
  if (strcmp(ht.id,"sht")){
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
  SHT_info *info = malloc(sizeof(SHT_info));

  /* We initialize its' dictionary where all the names will be stored. */
  info->name_to_id = (Dictionary*)malloc(sizeof(Dictionary));
  /* We initialize its' data. */
  initialize(info->name_to_id);

  info->Sfilename = sfileName;
  info->fileDesc = fileDesc;
  info->buckets = ht.buckets;
  info->lastRecord = (BF_BLOCK_SIZE) / sizeof(Record);
  info->blocks = blocks;

  /* We unpin the block. */
  CALL_OR_DIE(BF_UnpinBlock(block));

  /* We destroy the block. */
  BF_Block_Destroy(&block);

  /* We return the info block. */
  return info;
}

int SHT_CloseSecondaryIndex(SHT_info *header_info){
  /* We close the file with the file descriptor we get from
  the info block given as a parameter. */
  CALL_OR_DIE(BF_CloseFile(header_info->fileDesc));
  return SHT_OK;
}

int SHT_SecondaryInsertEntry(SHT_info *header_info, Record record, int block_id){

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
  int *posPointer = (int *)(dataOld + sizeof(HT_block_info) + block_id * sizeof(int));
  int pos = posPointer[0];

  /* If we are not in the block of the last record. */
  if (pos < header_info->lastRecord){
    /* We get the block given the position that we have to insert our new block. */
    CALL_OR_DIE(BF_GetBlock(header_info->fileDesc, block_id, blockNew));

    /* We get the data of the new block. */
    char *dataNew = BF_Block_GetData(blockNew);

    /* We insert the data in the block. */
    memcpy(dataNew + pos * sizeof(Record), &record, sizeof(Record));
    
    /* We insert the name of the record in the directory, while also inserting its
    coordinates in the memory. */
    insert(header_info->name_to_id,record.name,block_id,pos);

    /* We set the block dirty. */
    BF_Block_SetDirty(blockNew);

    /* We unpin the block. */
    CALL_OR_DIE(BF_UnpinBlock(blockNew));

    /* We destroy the block. */
    BF_Block_Destroy(&blockNew);
  }

  /* We initialize the new position of the block in order to insert it inside the
  old block. */
  pos++;
  memcpy(dataOld + 8 + block_id * 4, &pos, 4);

  /* We set the block dirty. */
  BF_Block_SetDirty(blockOld);

  /* We destroy the block. */
  BF_Block_Destroy(&blockOld);

  return SHT_OK;
}

int SHT_SecondaryGetAllEntries(HT_info *ht_info, SHT_info *header_info, char *name){

  int recordsTraversed = 0;

  /* We initialize the block that we are going to insert with the record. */
  BF_Block *blockNew;
  BF_Block_Init(&blockNew);

  /* We initialize the block where we need to insert the record at. */
  BF_Block *blockOld;
  BF_Block_Init(&blockOld);

  /* We get the data of the block that we need for the insertion. */
  char *dataOld;

  /* We get the first block of the file with the right file descriptor. */
  CALL_OR_DIE(BF_GetBlock(ht_info->fileDesc, 0, blockOld));

  /* We get the data of the block. */
  dataOld = BF_Block_GetData(blockOld);

  /* If no name was given, then we print all the records that exist inside the blocks. */
  if (name == NULL){
    /* We traverse all blocks. */
    for(int i = 1; i < header_info->blocks; i++){

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
  /* Otherwise we print all records that have name equal to the name of the record given
  as a parameter. */
  }else{
    /* We traverse all blocks. */
    for (int i = 1; i < header_info->blocks + 1; i++){
      /* We calculate the block position for the current block. */
      int *posPointer = (int *)(dataOld + sizeof(HT_block_info) + i * sizeof(int));
      int pos = posPointer[0];
      /* We traverse all records inside the block. */
      for (int j = 0; j < pos; j++){

        /* We use the directory in order to check if name exists with the given coordinates.
        If it exists, then we move on our functionality. This was implemented for the memory
        to be used as less times as possible. */
        if (find(header_info->name_to_id,name,i,j)){

          /* We get the block in this position. */
          BF_GetBlock(header_info->fileDesc, i, blockNew);

          /* We get the data inside the block. */
          char *data = BF_Block_GetData(blockNew);

          /* We get the record data inside the block. */
          Record rec;
          memcpy(&rec, data + j * sizeof(Record), sizeof(Record));

          /* We update the total amount of records that we traversed,
          in order for it to be returned at the end of execution. */
          recordsTraversed++;

          /* Print the record. */
          printRecord(rec);
        }
      }
      /* We unpin the block. */
      CALL_OR_DIE(BF_UnpinBlock(blockNew));
    }
  }

  /* We unpin the entire block. */
  CALL_OR_DIE(BF_UnpinBlock(blockOld))

  /* We also destroy the entire block. */
  BF_Block_Destroy(&blockOld);

  /* We return the total amount of records that were traversed. */
  return recordsTraversed;
}