#include "fileReader.h"

//calosc nie testowana
//nalezy poddac testom

unsigned char taken, next;
unsigned char emptyEndBitCount;
int bitPoint;
int bitsTillEnd;
FILE * write;

//nie ma sensu

//count: 1-8
//zwraca ile bitow pobral
int TakeMultibitFromFile(int count,unsigned int * out)
{
    *out=0;
    int i;
    unsigned char tmp;
	for(i=0;i<count;i++)
    {
if(TakeBitFromFile(&tmp)==0)return i;
        *out*=2;
        *out+=tmp;
    }
	return count;
}

//powinno uwzgledniac dopisane bity w ostatnim bajcie
//1-pobrano bit
//0-brak bitów
int TakeBitFromFile(unsigned char *out)
{
    *out=0;
    if(bitsTillEnd>0)bitsTillEnd--;
    if(bitsTillEnd==0)return 0;
    if(bitPoint==8)
    {
        if(fread(&next,1,1,write)==0&&bitsTillEnd<0)bitsTillEnd=8-emptyEndBitCount;
        taken=next;
        bitPoint=0;
    }
   
    //printf("bitPoint:%d bitsTillEnd:%d\n",bitPoint,bitsTillEnd);
    bitPoint++;
    *out = taken>127?1:0;
    taken<<=1;
    return 1;
}

//0 means empty file
int InitReadFile(FILE * file)
{
	bitPoint=0;
	write = file;
	taken= 0;
	bitsTillEnd=-1;
	return fread(&taken,1,1,write);
}

void SetEmptyEndBitCount(unsigned char s)
{
    emptyEndBitCount = s;
}
