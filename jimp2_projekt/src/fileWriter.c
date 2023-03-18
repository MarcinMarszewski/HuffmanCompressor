#include "fileWriter.h"


unsigned char cache;
long long int totalBitCount;
int bitwiseCount;
FILE * write;

int WriteIntWrap(int count, int in)
{
    int help=count/8;
    if(count*8!=help)WriteCharToFile(count-(help*8),(unsigned char)(in>>(help*8)));
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

