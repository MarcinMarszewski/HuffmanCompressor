#include "treeWriter.h"

int wordSize;
unsigned char charTmp;
unsigned int intTmp;

int WriteTree(node_t* head)
{
	if(head->left==NULL)
	{
		WriteCharToFile(1,1);
		WriteIntWrap(wordSize,head->value);
		return 1;
	}
	WriteCharToFile(1,0);
	WriteTree(head->left);
	WriteTree(head->right);	
	return 0;
}

int ReadTree(node_t* head)
{
	if(TakeBitFromFile(&charTmp)==0) return -1;
	printf("%d\n",charTmp);
	if(charTmp==0)
	{
		head->left = malloc( sizeof(head->left));
		ReadTree(head->left);
		head->right = malloc(sizeof(head->right));
		ReadTree(head->right);
	}
	else
	{
		TakeMultibitFromFile(wordSize,&intTmp);
		head->value = intTmp;
		head->left=NULL;
		printf("%d\n",head->value);
	}
}

int WriteTreeFillBite(node_t * head)
{
	WriteTree(head);
	WriteCharToFile(8-GetWriteBitwiseCount(),0);
}

int ReadTreeFillBite(node_t* head)
{
	ReadTree(head);
	TakeMultibitFromFile(8-GetReadBitwiseCount(),&intTmp);
}
void SetWordSize(int n)
{
	wordSize=n;
}
