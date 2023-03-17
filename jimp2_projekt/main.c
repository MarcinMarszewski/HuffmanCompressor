#include <stdio.h>
#include <stdlib.h>
#include "node.h"


int main(int args, char **argv) {
	
	FILE *in = args > 1 ? fopen(argv[1], "r") : stdin;
	if(in == NULL) {	
		fprintf(stderr, "Error: Cannot open infile \"%s\"\n", argv[1]);
		return EXIT_FAILURE;
	}

	dynamicArray *nodes = makeDynamicArray( 8 );	

	char x;
	while( ( x = (char)fgetc( in ) ) !=EOF ) {
		int bylo = 0;
		for(int i = 0; i < nodes->n; i++) {
			if( nodes->t[i].value == x ) {
				bylo = 1;
				nodes->t[i].quantity++;
				break;
			}
		}
		if( bylo == 0 )
			add( nodes, x );
	}
	

	 //TEST LISCI
	for(int i = 0; i < nodes->n; i++)
		printf("Znak: %c - %d razy\n", nodes->t[i].value, nodes->t[i].quantity);

	/*
	for(int i = 0; i < nodes->size; i++)
                printf("Wezel nr %d - %d\n", i, nodes->t[i].quantity);
	*/

	//makeTree( nodes );

	return EXIT_SUCCESS;
}
