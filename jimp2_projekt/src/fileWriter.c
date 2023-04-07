#include "fileWriter.h"

unsigned char cache;
long long int totalBitCount;
int bitwiseCount;
FILE * write;
char decode;

int WriteIntWrap(int count, int in)
{
    int help=count/8;
    if(help*8!=count)WriteCharToFile(count-(help*8),(unsigned char)(in>>(help*8)));
    int i=(help-1)*8;
    
    while(i>=0)
    {
        WriteCharToFile(8,(unsigned char)(in>>i));
    	i-=8;
    }
    
    return bitwiseCount;
}

int WriteCharToFile(int count, unsigned char in)
{
	int i;

	in<<=(8-count);
	for(i=0;i<count;i++)
	{
		cache=cache<<1;
		if(in>127)
			cache++;
		
		in<<=1;
		bitwiseCount++;
		totalBitCount++;
		
		if(bitwiseCount==8)
		{
			bitwiseCount=0;
			cache = cache^decode;
			fwrite(&cache,1,1,write);
		}
	}

	return bitwiseCount;
}

int GetWriteBitwiseCount()
{
	return bitwiseCount;
}

void InitFile(FILE * file)
{
	decode=0;
	totalBitCount=0;
	bitwiseCount=0;
	write = file;
	cache= '\0';
}

void SetDecode(char dec)
{
	decode=dec;
}

