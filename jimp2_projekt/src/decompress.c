#include "decompress.h"

void DecompressData(node_t * head,int wordLength)
{
	int i=0;
	unsigned char temp;
	node_t * nodeTemp=head;
	while(TakeBitFromFile(&temp)==1)
	{
		//i++;
		//printf("bit taken: %d\n",i);
		if(temp==0)
		{
			nodeTemp=nodeTemp->left;
		}
		else 
		{
			nodeTemp=nodeTemp->right;
		}

		if(nodeTemp->left==NULL)
		{
			//printf("null check: %d\n", nodeTemp->value);
			//printf("%d ",nodeTemp->value);
			WriteIntWrap(wordLength,nodeTemp->value);
			nodeTemp=head;
		}
	}
}