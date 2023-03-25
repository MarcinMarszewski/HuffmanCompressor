#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "key.h"
#include "fileWriter.h"
#include "fileReader.h"
#include "compress.h"

int main(int args, char **argv) {

	FILE *in = args > 1 ? fopen(argv[1], "rb") : stdin;
	if(in == NULL) {
		fprintf(stderr, "Error: Cannot open infile \"%s\"\n", argv[1]);
		return EXIT_FAILURE;
	}

	dynamicArray *nodes = makeDynamicArray( 8 );

	leavesMaker_8_16(in, nodes, 1);         	//tworzy tablice lisci dla 8 bitow

	fclose(in);

	makeTree( nodes );
	
	key_type *keys;
	keys = InitKeyArray(256);  //65536
	AssignKeys(*nodes->t[nodes->n -1], keys,0,0);

	FILE *in1 = args > 1 ? fopen(argv[1], "rb") : stdin;

	FILE *out = args > 2 ? fopen(argv[2], "wb") : stdout;
	if(out == NULL) {
		fprintf(stderr, "Error: Cannot open outfile \"%s\"\n", argv[2]);
		return EXIT_FAILURE;
	}
	
	compressToFile_8_16(in1, out, 1, keys);

	printf("\n");
	fclose(out);
	
	return EXIT_SUCCESS;
}
