#ifndef _COMPRESS_
#define _COMPRESS_

#include "node.h"
#include "key.h"

void leavesMaker_8_16 (FILE *, dynamicArray *, int);
void leavesMaker_12 (FILE *, dynamicArray *);
void compressToFile_8_16(FILE *, FILE *, int, key_type *);
void compressToFile_12(FILE *, FILE *, key_type *);


#endif