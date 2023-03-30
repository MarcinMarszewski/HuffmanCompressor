#include "treeWriter.h"

int wordSize;
unsigned char charTmp;
unsigned int intTmp;
int WriteTree(node_t* head)
{
	if(head->left==NULL)
	{
		WriteCharToFile(1,1);
		printf("%d\n",head->value);
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
	char t = TakeBitFromFile();
	if(t==0)
	{
		head->left = malloc( sizeof(head->left));
		ReadTree(head->left);
		head->right = malloc(sizeof(head->right));
		ReadTree(head->right);
	}
	else
	{
		head->value=TakeMultibitFromFile(wordSize);
		head->left=NULL;
	}
}

int WriteTreeFillBite(node_t * head)
{
	WriteTree(head);
	WriteCharToFile(GetWriteBitwiseCount()==8?0:8-GetWriteBitwiseCount(),0);
}

int ReadTreeFillBite(node_t* head)
{
	ReadTree(head);
	FillBite();
}
void SetWordSize(int n)
{
	wordSize=n;
}
