#ifndef FILEREADER
#define FILEREADER

#include <stdio.h>
#include <stdlib.h>

int TakeMultibitFromFile(int count);

char TakeBitFromFile(void);

void FillBite();

char CanRead();

int InitReadFile(FILE * file);

void SetEmptyEndBits(char ends);

#endif // FILEREADER
