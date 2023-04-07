#include "decompress.h"

void DecompressData(node_t * head,int wordLength)
{
	int i=0;
	char temp;
	
	node_t * nodeTemp=head;
	while(CanRead()==1)
	{
		i++;
		temp = TakeBitFromFile();

		if(temp==0)
			nodeTemp=nodeTemp->left;
		else 
			nodeTemp=nodeTemp->right;

		if(nodeTemp->left==NULL)
		{
			WriteIntWrap(wordLength,nodeTemp->value);
			nodeTemp=head;
		}
	}
}
