#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "key.h"
#include "fileWriter.h"
#include "fileReader.h"

int main(int args, char **argv) {

	FILE *in = args > 1 ? fopen(argv[1], "rb") : stdin;
	if(in == NULL) {
		fprintf(stderr, "Error: Cannot open infile \"%s\"\n", argv[1]);
		return EXIT_FAILURE;
	}

	dynamicArray *nodes = makeDynamicArray( 8 );

	unsigned char x;
	while( fread(&x, 1, 1, in ) == 1){
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

	/*printf("WEZLY PRZED:\n");
	for(int i = 0; i < nodes->n; i++)
		printf("Wezel nr: %d, Znak: %c - %d razy, wskaznik swoj: %p, wskaznik upper: %p\n\n", i, nodes->t[i]->value, nodes->t[i]->quantity, nodes->t[i] ,nodes->t[i]->upper );
	*/

	makeTree( nodes );

	/*printf("WEZLY PO:\n");
	for(int i = 0; i < nodes->n; i++)
        printf("Wezel nr: %d, Znak: %c - %d razy, wskaznik swoj: %p, wskaznik upper: %p\n\n", i, nodes->t[i]->value, nodes->t[i]->quantity, nodes->t[i], nodes->t[i]->upper );
	*/
	key_type *keys;
	keys = InitKeyArray(256);
	AssignKeys(*nodes->t[nodes->n -1], keys,0,0);

	
	FILE *out = args > 2 ? fopen(argv[2], "wb") : stdout;
	if(out == NULL) {
		fprintf(stderr, "Error: Cannot open outfile \"%s\"\n", argv[2]);
		return EXIT_FAILURE;
	}
	InitFile(out);
	WriteIntWrap(8,'b');
	WriteCharToFile(8, 'b');
	/*WriteCharToFile(4,1);
	
	WriteIntWrap(16,20560);
	WriteIntWrap(4,4);
	WriteCharToFile(4,15);
	

	int i;
	for(i=0;i<256;i++)
		printf("%d: %d %d %s\n", i, keys[i].value, keys[i].length, KeyToCode(keys[i]));
		//WriteIntWrap(16,16705);

	

	unsigned char t;
	out= fopen("output.txt","rb");
	InitReadFile(out);
	SetEmptyEndBitCount(0);
	while(TakeBitFromFile(&t)==1)
		printf("%d",t);
	*/
	printf("\n");

	fclose(out);
	return EXIT_SUCCESS;
}
