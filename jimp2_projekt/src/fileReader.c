#include "fileReader.h"

int pointer;
FILE * read;
char tmp;

//nie ma sensu

//count: 1-8
//zwraca ile bitow pobral
int TakeMultibitFromFile(int count)
{
    	int out=0;
    	int i;
	for(i=0;i<count;i++)
    	{
		out*=2;
		out+=TakeBitFromFile();
	}
	return out;
}
//bierze bit, nie patrzy na koniec pliku
char TakeBitFromFile()
{
	if(pointer==8)
	{
		pointer=0;
		fread(&tmp,1,1,read);
	}
	pointer++;
	if(tmp<0)
	{
		printf("1");
		tmp<<=1;
		return 1;
	}
	printf("0");
	tmp<<=1;
	return 0;
}

void FillBite()
{
	pointer=8;
}

//0 means empty file
int InitReadFile(FILE * file)
{
	pointer=0;
	read=file;
	fread(&tmp,1,1,read);
}
/*
void SetEmptyEndBitCount(char s)
{
    emptyEndBitCount = s;
}

int GetReadBitwiseCount()
{
	return bitPoint;
}

int ReadBit(char *n)
{
	*n=0;
	if(bitPoint==8)
	{
		fread(&taken,1,1,read);
		bitPoint=0;
	}
	if(taken<0)*n=1;
	else *n=0;
	bitPoint++;
	return 1;
}
*/
