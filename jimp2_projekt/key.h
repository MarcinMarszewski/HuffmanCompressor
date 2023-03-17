#ifndef KEY
#define KEY

#include <stdlib.h>
#include "node.h"

typedef struct key_type
{
	int value;
	int length;
} key_type;

key_type *InitKeyArray(int len);

void AssignKeys(node_t head, key_type *keys, int val, int len);
char* KeyToCode(key_type key);

#endif // KEY
