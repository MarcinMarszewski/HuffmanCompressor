#include "key.h"

//przetwarza drzewo w tablice kluczy
void AssignKeys(node_t head, key_type *keys, long int val, int len, int isVerbose) // uzycie z val=0, len=0
{
	char* tmp;
	if(head.left!=NULL) 
	{
		AssignKeys(*(head.left), keys, val*2, len+1, isVerbose);
		AssignKeys(*(head.right), keys, (val*2)+1, len+1, isVerbose);
	}
	else
	{
		keys[head.value].value = val;
		keys[head.value].length = len;
		if(isVerbose==1){
			tmp = KeyToCode(keys[head.value]);
			printf("Wartosc liscia: %d,  Kod do kompresji dziesietnie: %ld binarnie: %s\n", head.value, val, tmp);
			free(tmp);
		}
	}
}

//zmien key na klucz
char* KeyToCode(key_type key)
{
	char *ret= calloc(key.length, sizeof(*ret));
	int i;
	
	for(i = key.length-1;i>-1;i--)
	{
		if(key.value%2==0)
			ret[i]='0';
		else
		{
			ret[i]='1';
			key.value--;
		}
		key.value/=2;
	}
	
	return ret;
}

