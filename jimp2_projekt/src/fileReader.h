#ifndef FILEREADER
#define FILEREADER

#include "fileWriter.h"
#include <stdio.h>
#include <stdlib.h>

int TakeMultibitFromFile(int count,unsigned int * out);

//powinno uwzgl�dnia� dopisane bity w ostatnim bajcie
//1-pobrano bit
//0-brak bit�w
int TakeBitFromFile(unsigned char *out);

int InitReadFile(FILE * file);

void SetEmptyEndBitCount(unsigned char s);

int GetReadBitwiseCount();

#endif // FILEREADER
