#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "key.h"
#include "fileWriter.h"
#include "fileReader.h"
#include "treeWriter.h"


int main(int args, char **argv) {
	printf("%ld\n",sizeof(int));

	FILE *in = args > 1 ? fopen(argv[1], "rb") : stdin;
	if(in == NULL) {
		fprintf(stderr, "Error: Cannot open infile \"%s\"\n", argv[1]);
		return EXIT_FAILURE;
	}

	dynamicArray *nodes = makeDynamicArray( 8 );

	char x;
	while( ( x = (char)fgetc( in ) ) != EOF ) {
		int bylo = 0;
		for(int i = 0; i < nodes->n; i++) {
			if( nodes->t[i]->value == x ) {
				bylo = 1;
				nodes->t[i]->quantity++;
				break;
			}
		}
		if( bylo == 0 )
			add( nodes, x );
	}


	printf("WEZLY PRZED:\n");
	for(int i = 0; i < nodes->n; i++)
		printf("Wezel nr: %d, Znak: %c - %d razy, wskaznik swoj: %p, wskaznik upper: %p\n\n", i, nodes->t[i]->value, nodes->t[i]->quantity, nodes->t[i] ,nodes->t[i]->upper );
	/*i
	for(int i = 0; i < nodes->size; i++)
                printf("Wezel nr %d - %d\n", i, nodes->t[i]->quantity);
	*/

	makeTree( nodes );

	key_type *keys;
	keys = InitKeyArray(256);
	AssignKeys(*nodes->t[nodes->n -1], keys,0,0);

	SetWordSize(8);

	FILE *out = fopen("output.txt","wb");
	InitFile(out);

	WriteTree(nodes->t[nodes->n -1]);
	WriteCharToFile(8,0);
	fclose(out);

	unsigned char t;
	FILE *r= fopen("output.txt","rb");
	InitReadFile(r);
	SetEmptyEndBitCount(0);

	while(TakeBitFromFile(&t)==1)
		printf("%d",t);

	//printf("WEZLY PO:\n");
	//for(int i = 0; i < nodes->n; i++)
        //        printf("Wezel nr: %d, Znak: %c - %d razy, wskaznik swoj: %p, wskaznik upper: %p\n\n", i, nodes->t[i]->value, nodes->t[i]->quantity, nodes->t[i], nodes->t[i]->upper );

	return EXIT_SUCCESS;
}
