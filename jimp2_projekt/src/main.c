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
//-1 Nie udało się otworzyć pliku
//-2 Nieodpowiedni stopień kompresji
//-3 Plik z 1 lub mniej bajtów
//-4 Plik uszkodzony lub XOR-owany bajt niepoprawny
//-5 
//-6 Brak hasła
//
// 3 Wywołanie pomocy
//
//EXIT_SUCCESS program wykonany w pełni
//

int main(int argc, char **argv) {

	int i, option;
	FILE* in;
	FILE *out;
	char* fileName  = malloc(100*sizeof(*fileName ));
	char* fileName2 = malloc(100*sizeof(*fileName2));
	unsigned char xordPassword=0,isCompressed=0,compression=8,uncompressed=0,leftover=0,xordFileCheck=0,compressionData,tmpA,tmpB, uncompressedData;

	while((option=getopt(argc,argv,"s:f:o:p:h"))!=-1)
	{
		switch(option)
		{
			case 's':
				strcpy(fileName2,optarg);
				break;
			case 'f':
				in = fopen(optarg,"rb");
				strcpy(fileName,optarg);
				break;

			case 'o':
				compression = atoi(optarg);
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
	
	if(compression!=8&&compression!=12&&compression!=16)
	{
		fprintf(stderr,"Nieodpowiednia długość słowa kompresji:%d\nWybierz z:8,12,16\n",compression);
		return-2;
	}

	if(strcmp(fileName,fileName2)==0)
	{
		printf("%s %s\n",fileName,fileName2);
		fprintf(stderr,"Plik wejściowy i wyjściowy o takiej samej nazwie\n");
		return -5;
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
			if(xordPassword==0)
			{
				fprintf(stderr,"Plik chroniony, należy podać hasło.\n");
				return -6;
			}
			isCompressed=1;
		}
		else if(tmpB=='O')
			isCompressed=1;
	}

	//Dokąd zrefaktoryzowane//
	printf("Pass: %d\n",xordPassword);
	fread(&compressionData,1,1,in);
	fread(&xordFileCheck,1,1,in);	//wczytanie pierwszych 5 bajtów metadanych
	fread(&uncompressedData,1,1,in);
	printf("dekompresja nieskompresowane: %d\n",uncompressedData);
	if(isCompressed==1)
	{
		/*/XOR-owanie przed dekompresją
		while(fread(&tmpB, 1, 1, in) == 1)
			xordFileCheck ^= tmpB;
		fclose(in);
		if(xordFileCheck != 0){
			printf("Skompresowany plik jest uszkodzony! - wartosc: %d\n", xordFileCheck);
			return -4;
		}
		else
			printf("Plik nieuszkodzony!\n");

		in = fopen(fileName, "rb");
		fseek(in, 5, SEEK_SET);*/
		

		//  pozyskiwanie danych o kompresji
		compression=(1+(compressionData%4))*4;
		compressionData>>=2;
		uncompressed=(compressionData%4)*4;
		compressionData>>=2;
		leftover=compressionData%8;
		
		printf("compression:%d uncompressed:%d leftover:%d compressionData:%d\n",compression,uncompressed,leftover,compressionData);
	

		printf("Dekompresja\n");
		if(strlen(fileName2)==0)
		{
			strcpy(fileName2,fileName);
			strcat(fileName2,".decomp");
		}
		node_t *head = malloc(sizeof(*head));
		out= fopen(fileName2,"wb");

		SetWordSize(compression);

		InitReadFile(in);
		InitFile(out);
		SetEmptyEndBits(leftover);
		SetReadDecode(xordPassword);

		ReadTreeFillBite(head);
		SetReadDecode(0);
		DecompressData(head,compression);
		WriteCharToFile(uncompressed,uncompressedData);

		fclose(in);
		fclose(out);

	}
	else
	{
		printf("Kompresja\n");

		fclose(in);
	
		in = fopen(fileName,"rb");
		if(strlen(fileName2)==0)
		{
			strcpy(fileName2,fileName);
			strcat(fileName2,".squish");
		}

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
		unsigned char tempRest;

		switch(compression)//leftover uncompressed compression
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
				SetDecode(xordPassword);
				WriteTreeFillBite(nodes->t[nodes->n-1]);
				SetDecode(0);
				leftover = 8-compressToFile_8_16(in,out,1,keys);
				fclose(in);
			break;

			case 12:
				uncompressed = leavesMaker_12(in, nodes, &tempRest);
				fclose(in);
				in = fopen(fileName,"rb");
				makeTree(nodes);

				keys = InitKeyArray(4096);
				AssignKeys(*nodes->t[nodes->n-1],keys,0,0);
				SetWordSize(12);

				InitFile(out);
				SetDecode(xordPassword);
				WriteTreeFillBite(nodes->t[nodes->n-1]);
				SetDecode(0);
				leftover = 8-compressToFile_12(in,out,keys);
				fclose(in);
			break;

			case 16:
				uncompressed = leavesMaker_16(in,nodes, &tempRest);
				fclose(in);
				in = fopen(fileName,"rb");
				makeTree(nodes);

				keys = InitKeyArray(65536);
				AssignKeys(*nodes->t[nodes->n-1],keys,0,0);
				SetWordSize(16);

				InitFile(out);
				SetDecode(xordPassword);
				WriteTreeFillBite(nodes->t[nodes->n-1]);
				SetDecode(0);
				leftover = 8-compressToFile_8_16(in,out,2,keys);
				fclose(in);
			break;
		} //kompresja

		compressionData = 0;
		compressionData+=leftover;
		compressionData<<=2;
		compressionData+=uncompressed/4;
		compressionData<<=2;
		compressionData+=(compression/4)-1;
		printf("kompresja nieskompresowane: %d\n",tempRest);
		
		printf("compression:%d uncompressed:%d leftover:%d compressionData:%d \n",compression,uncompressed,leftover, compressionData);
		fseek(out,2,SEEK_SET);
		fwrite(&compressionData,1,1,out);
		fseek(out,4,SEEK_SET);
		fwrite(&tempRest,1,1,out);
		fclose(out);

		/*XOR-owanie po kompresjii
		in = fopen(fileName2, "rb");
		fseek(in,5,SEEK_SET);
		xordFileCheck = 0;
		while(fread(&tmpB, 1, 1, out) == 1)
			xordFileCheck ^= tmpB;
		fclose(in);

		out = fopen(fileName2, "rb+");
		fseek(out, 3, SEEK_SET);
		fwrite(&xordFileCheck, 1, 1, out);
		fclose(out);*/


		//zapisywanie metadanych
		freeDynamicArray(nodes);
	}

	free(fileName);
	free(fileName2);

	return EXIT_SUCCESS;
}
