#ifndef FILEWRITER
#define FILEWRITER

#include <stdio.h>
#include <stdlib.h>

int WriteIntWrap(int count, int in);

int WriteCharToFile(int count, unsigned char in);

void InitFile(FILE *in);

int GetWriteBitwiseCount();

#endif
