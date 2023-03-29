#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "key.h"
#include "compress.h"

//MAKING LEAVES FROM FILE

//8 and 16 bits
void leavesMaker_8_16 (FILE *in, dynamicArray* nodes, int bytes ) {     //bytes - 1 for 8 bits, 2 for 16 bites
    unsigned short x = 0;
	while( fread(&x, 1, bytes, in ) == bytes){
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
}

//12 bits
void leavesMaker_12 (FILE *in, dynamicArray *nodes) {
	unsigned short x1 = 0;
	unsigned short x2 = 0;
	unsigned char tmp = 0;
	int check = 3;

	while(check == 3){
		x1 = 0;
		x2 = 0;
		tmp = 0;
		check = 0;

    	if( fread(&x1, 1, 1, in) == 1)
			check++;
		x1 = x1<<8;
		if( fread(&tmp, 1, 1, in) == 1)
			check++;
		x1 = x1 | (tmp&240);
		x1 = x1>>4;
		x2 = x2 | (tmp&15);
		x2 = x2<<8;
		if( fread(&tmp, 1, 1, in) == 1)
			check++;
		x2 = x2 | tmp;

		int byloX1 = 0;
		int byloX2 = 0;
		for(int i = 0; i < nodes->n; i++) {
			if( (nodes->t[i]->value == x1) && (check >=2) ) {
				byloX1 = 1;
				nodes->t[i]->quantity++;
			}
			else if ( (nodes->t[i]->value == x2) && (check == 3) )
			{
				byloX2 = 1;
				nodes->t[i]->quantity++;
			}
		}
		if( (byloX1 == 0) && (check >=2) )
			add( nodes, x1 );
		if( (byloX2 == 0) && (check == 3) )
			add( nodes, x2 );
		
	} 
}

//COMPRESSING DATA FROM FILE IN TO FILE OUT

//8 and 16 bits
void compressToFile_8_16(FILE *in, FILE *out, int bytes, key_type *keys) {

    unsigned short x = 0;
    char *buff = calloc( 64, sizeof( *buff ) );
    unsigned char y = 0;

    while( fread(&x, 1, bytes, in ) == bytes){
		strcat(buff, KeyToCode( keys[x] ) );

        while( buff[7] != '\0' ){

            for(int i=7; i>=0; i--) {
                if( buff[i] == '1')
                    y |= 1 << (7 - i);
            }

            fwrite(&y, 1, 1, out);
            y=0;

            for(int i=0; i<58; i++)
                buff[i] = buff[i+8];
        } 
	}

    free(buff);
}

//12 bits
void compressToFile_12(FILE *in, FILE *out, key_type *keys) {
   
    unsigned short x1 = 0;
	unsigned short x2 = 0;
	unsigned char tmp = 0;
	int check = 3;
    char *buff = calloc( 64, sizeof( *buff ) );
    unsigned char y = 0;

    while(check == 3){
		x1 = 0;
		x2 = 0;
		tmp = 0;
		check = 0;

    	if( fread(&x1, 1, 1, in) == 1)
			check++;
		x1 = x1<<8;
		if( fread(&tmp, 1, 1, in) == 1)
			check++;
		x1 = x1 | (tmp&240);
		x1 = x1>>4;
		x2 = x2 | (tmp&15);
		x2 = x2<<8;
		if( fread(&tmp, 1, 1, in) == 1)
			check++;
		x2 = x2 | tmp;

        if(check >= 2) 
            strcat(buff, KeyToCode( keys[x1] ) );
        if(check == 3)
            strcat(buff, KeyToCode( keys[x2] ) );
		
        while( buff[7] != '\0' ){

            for(int i=7; i>=0; i--) {
                if( buff[i] == '1')
                    y |= 1 << (7 - i);
            }

            fwrite(&y, 1, 1, out);
            y=0;

            for(int i=0; i<58; i++)
                buff[i] = buff[i+8];
        }
	} 
    free(buff);
}