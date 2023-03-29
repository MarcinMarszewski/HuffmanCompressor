#include "decompress.h"

void DecompressData(node_t * head,int wordLength)
{
	int i=0;
	char temp;
	node_t * nodeTemp=head;
	while(i<10000)
	{
		i++;
		temp = TakeBitFromFile();
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
