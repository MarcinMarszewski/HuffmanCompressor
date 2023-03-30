#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "node.h"
#include "key.h"
#include "fileWriter.h"
#include "fileReader.h"
#include "treeWriter.h"
#include "compress.h"
#include "decompress.h"

//kody zwrotów i błędów
//-1 nie udało się otworzyć pliku
//-2 nieodpowiedni stopień kompresji
//-3 plik z 1 lub mniej bajtów
//
// 3 wywołanie pomocy
//EXIT_SUCCESS program wykonany w pełni
//

int main(int argc, char **argv) {

	/*
	int option, compression, i;
	FILE* in;
	FILE *out;
	char* fileName;
	char xordPassword=0,isCompressed=0,isProtected=0,xordFileCheck,compressionData,tmpA,tmpB;

	while((option=getopt(argc,argv,"f:o:p:h"))!=-1)
	{
		switch(option)
		{
			case 'f':
				in = fopen(optarg,"rb");
				fileName= malloc((strlen(optarg)+10)*sizeof(*fileName));
				strcpy(fileName,optarg);
				if(in==NULL) 
				{
					fprintf(stderr,"Nie udało się otworzyć pliku %s\n",optarg);
					return -1;
				}
				break;

			case 'o':
				compression = atoi(optarg);
				if(compression!=8&&compression!=12&&compression!=16)
				{
					fprintf(stderr,"Niedopowiednia długość słowa kompresji:%d\nWybierz z:8,12,16\n",compression);
					return -2;
				}
				break;

			case 'p':
				for(i=0;i<strlen(optarg);i++)xordPassword=xordPassword^optarg[i];
				break;

			case 'h':
				fprintf(stdout,"Pomoc w obsłudze kompresora:\n");
				return 3;
				break;

			case '?':
				fprintf(stderr,"Nieznany argument: %c\n",optopt);
				break;
		}
	}
	
	if(fread(&tmpA,1,1,in)==0||fread(&tmpB,1,1,in)==0)
	{
		fprintf(stderr,"Plik zawiera 1 lub mniej bajtów\n");
		return -3;
	}

	if(tmpA=20)
	{
		if(tmpB='P')
		{
			isProtected=1;
			isCompressed=1;
		}
		else if(tmpB='O')
			isCompressed=1;
	}
	
	fread(&compressionData,1,1,in);
	fread(&xordFileCheck,1,1,in);	//wczytanie pierwszych 4 bajtów metadanych

	if(isProtected==1)
	{
		//dekompresja z hasłem
	
	}
	else if(isCompressed==1)
	{
		//dekompresja bez hasła
	}
	else
	{
		fclose(in);
		in = fopen(fileName,"rb");

		strcat(fileName,".squish");
		out = fopen(fileName,"wb");
		
		tmpA=20;
		fwrite(&tmpA,1,1,out);
		if(xordPassword==0)tmpA='O';
		else tmpA='P';
		fwrite(&tmpA,1,1,out);
		tmpA=0;
		fwrite(&tmpA,1,1,out);
		fwrite(&tmpA,1,1,out); //zapisanie 4 startowych bajtów
		
		
		//kompresja
	}

	*/
		/*

		FILE* in = fopen(argv[1],"rb");
		InitReadFile(in);
		int i;
		for(i=0;i<10;i++)
		{
			printf("%d\n",TakeMultibitFromFile(8));
		}*/

	
	if(atoi(argv[1])==0)	//0 - kompresja
	{	
	FILE *in = argc > 2 ?  fopen(argv[2], "rb") : stdin;
	FILE *compressed = argc > 3 ? fopen(argv[3],"wb") : stdout;

	if(in == NULL) {
		fprintf(stderr, "Error: Cannot open infile \"%s\"\n", argv[1]);
		return EXIT_FAILURE;
	}


	dynamicArray *nodes = makeDynamicArray( 8 );

	leavesMaker_8_16(in, nodes, 1);         	//tworzy tablice lisci dla 8 bitow
	fclose(in);
	in = fopen(argv[2],"rb");

	makeTree( nodes );
	
	key_type *keys;
	keys = InitKeyArray(256);  //65536
	AssignKeys(*nodes->t[nodes->n -1], keys,0,0);
	
	SetWordSize(8);

	InitFile(compressed);
	WriteTreeFillBite(nodes->t[nodes->n-1]);

	compressToFile_8_16(in,compressed,1,keys);

	fclose(in);
	fclose(compressed);
	printf("Compressed\n");
	}
	else
	{
	node_t* head= malloc(sizeof(*head));
	printf("malloced\n");
	FILE *compressed = argc>2?fopen(argv[2],"rb"):stdin;
	FILE *decompressed = argc>3?fopen(argv[3],"wb"):stdout;


	printf("File opened\n");
	SetWordSize(8);

	InitReadFile(compressed);
	InitFile(decompressed);
	printf("Files initiated\n");
	SetEmptyEndBits(0);
		
	ReadTreeFillBite(head);
	printf("Tree Read\n");
	DecompressData(head,8);
	printf("Data decompressed\n");
		
	fclose(compressed);
	fclose(decompressed);
	}	

	return EXIT_SUCCESS;
}
