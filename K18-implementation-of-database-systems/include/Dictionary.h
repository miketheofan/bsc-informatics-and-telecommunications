#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Dictionary Dictionary;

struct Dictionary {     
    char *name;
    int x;
    int y;
    Dictionary *next;
};

void initialize(Dictionary*);
void insert(Dictionary*,char*,int,int);
bool find(Dictionary*,char*,int,int);

#endif