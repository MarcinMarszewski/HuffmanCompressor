#include "fileWriter.h"

//ca³oœæ nie testowana
//nale¿y poddaæ testom

unsigned char taken, next;
unsigned char emptyEndBitCount;
int bitPoint;
int bitsTillEnd;
FILE * write;

//nie ma sensu

//count: 1-8
//zwraca ile bitow pobral
int TakeMultibitFromFile(int count,unsigned char * out)
{
    *out='\0';
    int i;
    unsigned char tmp;
	for(i=0;i<count;i++)
    {
        if(TakeBitFromFile(&tmp)==0)return i;
        out*=2;
        out+=tmp;
    }
	return count;
}

//powinno uwzglêdniaæ dopisane bity w ostatnim bajcie
//1-pobrano bit
//0-brak bitów
int TakeBitFromFile(unsigned char *out)
{
    *out='\0';
    if(bitPoint==8)
    {
        if(fread(&next,1,1,write)==0)bitsTillEnd=emptyEndBitCount==0?0:8-emptyEndBitCount;
        if(bitsTillEnd==0)return 0;
        if(bitsTillEnd>0)bitsTillEnd--;
        taken=next;
        bitPoint=0;
    }
    bitPoint++;
    *out = taken>127?1:0;
    taken<<=1;
    return 1;

}

void InitFile(FILE * file)
{
	bitPoint=8;
	write = file;
	taken= '\0';
	bitsTillEnd=-1;
}

void SetEmptyEndBitCount(unsigned char s)
{
    emptyEndBitCount = s;
}
