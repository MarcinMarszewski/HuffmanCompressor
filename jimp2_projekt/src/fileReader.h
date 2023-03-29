#ifndef FILEREADER
#define FILEREADER

#include <stdio.h>
#include <stdlib.h>

int TakeMultibitFromFile(int count);

char TakeBitFromFile(void);

void FillBite();

int InitReadFile(FILE * file);
/*
void SetEmptyEndBitCount(char s);

int GetReadBitwiseCount();

int ReadBit(char *t);
*/
#endif // FILEREADER
