#ifndef FILEREADER
#define FILEREADER

#include <stdio.h>
#include <stdlib.h>

int TakeMultibitFromFile(int count);

char TakeBitFromFile(void);

void FillBite();

char CanRead();

int InitReadFile(FILE * file);

void SetReadDecode(char dec);

void SetEmptyEndBits(char ends);

#endif // FILEREADER
