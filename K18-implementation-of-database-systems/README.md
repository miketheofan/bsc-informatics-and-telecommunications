# K18-Implementation of Database Systems

Projects for the course "_K18:Implementation of Database Systems_" 2022 by professor [Dimitrios Gounopoulos](https://www.di.uoa.gr/staff/46).

## General Information

The `Makefile` commands for running all three executables have remained unchanged from those provided in the assignment statement.

## HP (Heap File)

- **HP_info Structure:**
  - Maintains file descriptor and block entries for function arguments and persistent information during execution.
  
- **Heap File Creation:**
  - Each heap file is assigned a unique identifier "hp" for validation.

- **Block Management:**
  - All written blocks are marked as dirty; blocks no longer needed are unpinned.
  - Proper deallocation of `BF_Block` structures and dynamically allocated memory is ensured.

- **Record Insertion:**
  - Fields of a record are added serially in memory.

- **Block Capacity Calculation:**
  - An inline function in `hp_file.h` defines block capacity.

- **Main Function Usage:**
  - The main function and its usage follow the provided prototype.

- **HP_GetAllEntries Function:**
  - Checks for a specific value; if not provided, `PRINT_ALL` is used to return all entries.

## HT (Hash Table)

- **HT_info Structure:**
  - Maintains file name, file descriptor, bucket count, and last record location for persistent function arguments.

- **HT_block_info Structure:**
  - Contains a 3-character string identifier to distinguish HT structures.

- **File Initialization:**
  - The first block of each file contains "hp" and the bucket count.

- **Block Allocation:**
  - Subsequent blocks store the hashtable and are allocated in `HT_OpenFile`.

- **Record Insertion:**
  - Records are inserted into blocks, with positions computed based on block and existing records.

- **Block Management:**
  - Blocks with insertions are marked as dirty; unnecessary blocks are unpinned.

- **Record Printing:**
  - If `NULL` is passed as a value, all records are printed. Otherwise, specific blocks are searched and printed.

## SHT (Secondary Hash Table)

- **SHT_info Structure:**
  - Maintains primary and secondary hash file names, file descriptor, bucket count, last record location, total block count, and a dictionary of names.

- **SHT Process:**
  - Similar to HT, with the addition of a custom `Dictionary` structure to store record names and coordinates for efficient searching.

- **Record Insertion:**
  - Uses `block_id` from `HT_InsertEntry` to directly insert records into the correct location in SHT memory.

## General Notes

- **Memory Management:**
  - All `BF_Block` structures and dynamically allocated memory are properly deallocated.

- **Executable Testing:**
  - Modified `srand(time(NULL))` to ensure different values each run.
  - Added commands to print all records and specific records for testing purposes.

- **Return Types:**
  - Created `enum HP_ErrorCode/HT_ErrorCode/SHT_ErrorCode` for return values including `OK` and `ERROR`.

- **Code Documentation:**
  - Executable files contain sufficient comments for code explanation.
