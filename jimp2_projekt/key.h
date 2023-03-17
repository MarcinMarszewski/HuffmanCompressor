#include <stdlib.h>

#ifndef NODE
#include "node.h"
#define NODE
#endif
typedef struct key_type
{
	int value;
	int length;
} key_type;

key_type *InitKeyArray(int len);

void AssignKeys(node_t head, key_type *keys, int val, int len);
char* KeyToCode(key_type key);
