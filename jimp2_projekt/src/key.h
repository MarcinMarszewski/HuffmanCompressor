#ifndef KEY
#define KEY

#include <stdlib.h>
#include "node.h"

typedef struct key_type
{
	long int value;
	int length;
} key_type;

void AssignKeys(node_t head, key_type *keys, long int val, int len, int isVerbose);

char* KeyToCode(key_type key);

#endif // KEY
