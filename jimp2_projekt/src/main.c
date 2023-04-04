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
//-4 Plik uszkodzony lub XOR-owany bajt niepoprawny
//
// 3 wywołanie pomocy
//EXIT_SUCCESS program wykonany w pełni
//

int main(int argc, char **argv) {

	int i, option;
	FILE* in;
	FILE *out;
	char* fileName  = malloc(100*sizeof(*fileName ));
	char* fileName2 = malloc(100*sizeof(*fileName2));
	char xordPassword=0,isCompressed=0,isProtected=0,compression=8,uncompressed=0,leftover=0,xordFileCheck=0,compressionData,tmpA,tmpB;
	unsigned char uncompressedData;
	printf("Ustawienie zmiennych\n");
	
	//xordPassword=17;//tymczasowe testy

	while((option=getopt(argc,argv,"f:o:p:h"))!=-1)
	{
		switch(option)
		{
			case 'f':
				in = fopen(optarg,"rb");
				strcpy(fileName,optarg);
				strcpy(fileName2,optarg);
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

	if(in==NULL)
	{
		fprintf(stderr,"Nie udało się otworzyć pliku %s\n",fileName);
		return-1;
	}
	
	if(fread(&tmpA,1,1,in)==0||fread(&tmpB,1,1,in)==0)
	{
		fprintf(stderr,"Plik zawiera 1 lub mniej bajtów\n");
		return -3;
	}

	if(tmpA==20)
	{
		if(tmpB=='P')
		{
			isProtected=1;
			isCompressed=1;
		}
		else if(tmpB=='O')
			isCompressed=1;
	}
	
	fread(&compressionData,1,1,in);
	fread(&xordFileCheck,1,1,in);	//wczytanie pierwszych 4 bajtów metadanych
	fread(&uncompressedData,1,1,in);

	if(isProtected==1)
	{
		printf("Ustawienie wartości hasła\n");
	
	}
	if(isCompressed==1)
	{
		/*  pozyskiwanie danych o kompresji
		compression=(1+(compressionData%4))*4;
		compressionData>>=2;
		uncompressed=(compressionData%4)*4;
		compressionData>>=2;
		leftover=compressionData%8;
		*/


		printf("Dekompresja\n");

		node_t *head = malloc(sizeof(*head));
		strcat(fileName2,".decomp");
		out= fopen(fileName2,"wb");

		SetWordSize(compression);

		InitReadFile(in);
		InitFile(out);
		SetEmptyEndBits(leftover);
		SetDecode(xordPassword);
		
		ReadTreeFillBite(head);
		DecompressData(head,compression);
		WriteCharToFile(uncompressed,uncompressedData);
		fclose(in);
		fclose(out);

		printf("XOR-owanie po dekompresji\n");
		in = fopen(fileName2, "rb");
		while(fread(&tmpB, 1, 1, in) == 1)
			xordFileCheck ^= tmpB;
		fclose(in);
		if(xordFileCheck != 0){
			printf("Plik uszkodzony!\n");
			return -4;
		}
		else
			printf("Plik nieuszkodzony!\n");

	}
	else
	{
		printf("XOR-owanie przed kompresja\n");
		in = fopen(fileName, "rb");
		xordFileCheck = 0;
		while(fread(&tmpB, 1, 1, in) == 1)
			xordFileCheck ^= tmpB;

		printf("Kompresja\n");
		fclose(in);
		in = fopen(fileName,"rb");
		strcat(fileName2,".squish");
		out = fopen(fileName2,"wb");
		
		tmpA=20;
		fwrite(&tmpA,1,1,out);
		if(xordPassword==0)tmpA='O';
		else tmpA='P';
		fwrite(&tmpA,1,1,out);
		tmpA=0;
		fwrite(&tmpA,1,1,out);
		fwrite(&xordFileCheck,1,1,out); //zapisanie 5 startowych bajtów
		fwrite(&tmpA,1,1,out);

		dynamicArray *nodes = makeDynamicArray(8);
		key_type *keys;
		unsigned char tymczasowa_zmienna_przechowujaca_reszte;

		switch(compression)
		{
			case 8:
				printf("kompresja 8\n");
				leavesMaker_8(in,nodes);
				fclose(in);
				in = fopen(fileName,"rb");
				makeTree(nodes);

				keys = InitKeyArray(256);
				AssignKeys(*nodes->t[nodes->n-1],keys,0,0);
				SetWordSize(8);

				InitFile(out);
				SetReadDecode(xordPassword);
				WriteTreeFillBite(nodes->t[nodes->n-1]);
				compressToFile_8_16(in,out,1,keys);
				fclose(in);
				fclose(out);
			break;

			case 12:
				leavesMaker_12(in, nodes, tymczasowa_zmienna_przechowujaca_reszte);
				fclose(in);
				in = fopen(fileName,"rb");
				makeTree(nodes);
				
				keys = InitKeyArray(4096);
				AssignKeys(*nodes->t[nodes->n-1],keys,0,0);
				SetWordSize(12);

				InitFile(out);
				WriteTreeFillBite(nodes->t[nodes->n-1]);
				compressToFile_12(in,out,keys);
				fclose(in);
				fclose(out);
			break;

			case 16:
				leavesMaker_16(in,nodes, tymczasowa_zmienna_przechowujaca_reszte);
				fclose(in);
				in = fopen(fileName,"rb");
				makeTree(nodes);
				
				keys = InitKeyArray(65536);
				AssignKeys(*nodes->t[nodes->n-1],keys,0,0);
				SetWordSize(16);

				InitFile(out);
				WriteTreeFillBite(nodes->t[nodes->n-1]);
				compressToFile_8_16(in,out,2,keys);
				fclose(in);
				fclose(out);
			break;
		} //kompresja



		//zapisywanie metadanych
	}

		/*

		FILE* in = fopen(argv[1],"rb");
		InitReadFile(in);
		int i;
		for(i=0;i<10;i++)
		{
			printf("%d\n",TakeMultibitFromFile(8));
		}*/
	
	/*

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
	*/
	return EXIT_SUCCESS;
}
