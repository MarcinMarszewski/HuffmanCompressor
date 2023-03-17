#include "fileReader.h"


unsigned char cache;
long long int totalBitCount;
int bitwiseCount;
FILE * read;

int WriteIntWrap(int count, int in)
{
    int i;
    for(i=count;i>0;i-=8)
    {
    in>>=8;
    WriteCharToFile(8<count?8:count,(unsigned char)in);
    }
    return bitwiseCount;
}

int WriteCharToFile(int count, unsigned char in)
{
	in<<=(8-count);
	int i;
	for(i=0;i<count;i++)
	{
		cache=cache<<1;
		if(in>127)cache++;
		in<<=1;

		totalBitCount++;
		bitwiseCount++;
		if(bitwiseCount==8)
		{
			bitwiseCount=0;
			fwrite(&cache,1,1,write);
		}
	}
	return bitwiseCount;
}

void InitFile(FILE * file)
{
	totalBitCount=0;
	bitwiseCount=0;
	write = file;
	cache= '\0';
}

