#include "fileReader.h"

FILE * read;
char tmp, next;
char canRead;
char countTillEnd,leftover, pointer;

//count: 1-8
//zwraca podaną liczbę bitów w intcie
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

//zwraca następny bit w plliku
char TakeBitFromFile()
{
	if(pointer==8)
	{
		pointer=0;
		tmp=next;
		if(fread(&next,1,1,read)==0)countTillEnd=8-leftover;
	}
	if(countTillEnd>0)countTillEnd--;
	if(countTillEnd==0)canRead=0;
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

//przesuwa czytanie na następny bajt
void FillBite()
{
	//while(pointer!=0&&pointer!=8)TakeBitFromFile();
	pointer=8;
}

//inicjuje plik do czytania
int InitReadFile(FILE * file)
{
	countTillEnd=-1;
	leftover=0;
	canRead=1;
	pointer=0;
	read=file;
	fread(&tmp,1,1,read);
	fread(&next,1,1,read);
}

//zwraca 1 jeśli można wczytać następny bit
char CanRead()
{
	return canRead;
}

//ustawia ile bitów w ostatnim bajcie nie zawiera informacji
void SetEmptyEndBits(char ends)
{
	leftover=ends;
}
