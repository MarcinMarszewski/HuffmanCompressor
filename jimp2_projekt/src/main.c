#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "key.h"
#include "fileWriter.h"
#include "fileReader.h"
#include "treeWriter.h"
#include "compress.h"
#include "decompress.h"


int main(int args, char **argv) {

	if(atoi(argv[3])==0)	//0 - kompresja
	{	
	FILE *in = args > 1 ? fopen(argv[1], "rb") : stdin;
	FILE *compressed = args > 2 ? fopen(argv[2],"wb") : stdout;

	if(in == NULL) {
		fprintf(stderr, "Error: Cannot open infile \"%s\"\n", argv[1]);
		return EXIT_FAILURE;
	}


	dynamicArray *nodes = makeDynamicArray( 8 );

	leavesMaker_8_16(in, nodes, 1);         	//tworzy tablice lisci dla 8 bitow
	fclose(in);
	in = fopen(argv[1],"rb");

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
	FILE *compressed = fopen(argv[1],"rb");
	FILE *decompressed = fopen(argv[2],"wb");

	printf("File opened\n");

	FILE *in2 = fopen(argv[1], "rb");
	FILE *out2 = fopen(argv[2], "wb");

	unsigned char t;
	FILE *r= fopen("output.txt","rb");
	InitReadFile(r);
	SetEmptyEndBitCount(0);
	InitReadFile(compressed);
	InitFile(decompressed);
	printf("Files initiated\n");
		
	ReadTreeFillBite(head);
	printf("Tree Read\n");
	DecompressData(head,8);
	printf("Data decompressed\n");
		
	fclose(compressed);
	fclose(decompressed);
	}	




	//compressToFile_8_16(in,out,1,keys);
	
	//FILE COMPRESSION TO HERE
	
	/*
	fclose(out);
	out = fopen(argv[2],"rb");
	
	unsigned char t;
	FILE *dec= fopen("decompressed.txt","wb");
	InitReadFile(out);
	InitFile(dec);
	SetEmptyEndBitCount(0);
	printf("Initialized components\n");
	//ReadTreeFillBite(nodes->t[nodes->n-1]);
	printf("Read binary Tree\n");
	DecompressData(nodes->t[nodes->n-1],8);
	printf("Decompressed data\n");

	//printf("WEZLY PO:\n");
	//for(int i = 0; i < nodes->n; i++)
        //        printf("Wezel nr: %d, Znak: %c - %d razy, wskaznik swoj: %p, wskaznik upper: %p\n\n", i, nodes->t[i]->value, nodes->t[i]->quantity, nodes->t[i], nodes->t[i]->upper );
	fclose(in);
	fclose(out);
	fclose(dec);
	*/
	return EXIT_SUCCESS;
}
