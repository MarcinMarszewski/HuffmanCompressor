#include "key.h"

//inicjuje tablice kluczy
key_type *InitKeyArray(int len)
{
	key_type *keys = malloc( len * sizeof(*keys));
	return keys;
}

//przetwarza drzewo w tablice kluczy
void AssignKeys(node_t head, key_type *keys, int val, int len) // uzycie z val=0, len=0
{
	printf("quantity: %d  value: %d  val:%d  len:%d\n", head.quantity, head.value,val,len);
	if(head.left!=NULL) 
	{
		AssignKeys(*(head.left), keys, val*2, len+1);
		AssignKeys(*(head.right), keys, (val*2)+1, len+1);
	}
	else
	{
		keys[head.value].value = val;
		keys[head.value].length = len;
	}
}

//zmien key na klucz
char* KeyToCode(key_type key)
{
	char *ret= malloc(key.length * sizeof(*ret));
	int i;
	for(i = key.length-1;i>-1;i--)
	{
		if(key.value%2==0)
		{
			ret[i]='0';
		}
		else
		{
			ret[i]='1';
			key.value--;
		}
		key.value/=2;
	}
	return ret;
}

