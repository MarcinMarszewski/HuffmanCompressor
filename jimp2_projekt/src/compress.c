#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h> 

#include "key.h"
#include "compress.h"

//MAKING LEAVES FROM FILE

//8 bits
void printLeaves(dynamicArray* nodes){
	for(int i=0; i<nodes->n; i++) {
		printf("Lisc nr %d: wartosc:%d, ilosc wystapien:%d\n", i, nodes->t[i]->value, nodes->t[i]->quantity);
	}
}

void leavesMaker_8 (FILE *in, dynamicArray* nodes, int isVerbose) {     //bytes - 1 for 8 bits, 2 for 16 bites
    unsigned short x = 0;
	while( fread(&x, 1, 1, in ) == 1){
		int was = 0;
		for(int i = 0; i < nodes->n; i++) {
			if( nodes->t[i]->value == x ) {
				was = 1;
				nodes->t[i]->quantity++;
				break;
			}
		}
		if( was == 0 )
			add( nodes, x );
	}
	if(isVerbose==1)
		printLeaves(nodes);
}

//16 bits
int leavesMaker_16 (FILE *in, dynamicArray* nodes, unsigned char* rest, int isVerbose) {     
	
	unsigned short x = 0;
	unsigned short tmp = '\0';
	int check = 2;
	int was;
	*rest = 0;

	while( check == 2 ){
		x = 0;
		check = 0;
		tmp = '\0';

		if( fread(&x, 1, 1, in ) == 1 ) {
			check++;
			x = x << 8;
		}
		if( fread(&tmp, 1, 1, in ) == 1 ) { 
			check++;
			x |= (tmp&255);
		}
		was = 0;
		for(int i = 0; i < nodes->n; i++) {
			if( nodes->t[i]->value == x ) {
				was = 1;
				nodes->t[i]->quantity++;
				break;
			}
		}
		
		if( was == 0 && check == 2 )
			add( nodes, x );
			
	}
	if( check == 1) {
			*rest = x>>8;
			return 8;
	} 
	else if( check == 0 ){
		return 0;
	}
	if(isVerbose==1)
		printLeaves(nodes);
}

//12 bits
int leavesMaker_12 (FILE *in, dynamicArray *nodes, unsigned char* rest, int isVerbose) {
	unsigned short x1 = 0;
	unsigned short x2 = 0;
	int wasX1;
	int wasX2;
	unsigned char tmp = 0;
	int check = 3;
	*rest = 0;
	int i=0;
	while(check >= 3){
		x1 = 0;
		x2 = 0;
		tmp = '\0';
		check = 0;
		wasX1 = 0;
		wasX2 = 0;

    	if(fread(&x1, 1, 1, in) == 1){
			check++;
			x1 = x1<<8;
		}
		if(fread(&tmp, 1, 1, in) == 1){
			check++;
			x1 = x1 | (tmp&240);
			x1 = x1>>4;
			x2 = x2 | (tmp&15);
			x2 = x2<<8;
		}
		if(fread(&tmp, 1, 1, in) == 1){
			check++;
			x2 = x2 | tmp;
		}

		for(int i = 0; i < nodes->n; i++) {
			if( (nodes->t[i]->value == x1 ) && (check >= 2) ) {
				wasX1 = 1;
				nodes->t[i]->quantity++;
			}
			else if ( (nodes->t[i]->value == x2 ) && (check == 3) )
			{
				wasX2 = 1;
				nodes->t[i]->quantity++;
			}
		}
	
		if( (wasX1 == 0) && (check >= 2) )
			add( nodes, x1 );
		if( (wasX2 == 0) && (check == 3) )
			add( nodes, x2 );

	if(check == 0) {
		return 0;
	}
	else if(check == 1) {
		*rest = x1>>8;
		return 8;
	}
	else if(check == 2) {
		*rest = (*rest | tmp&15);
		return 4;
	}
	if(isVerbose==1)
		printLeaves(nodes);
	}
}

//COMPRESSING DATA FROM FILE IN TO FILE OUT

//8 and 16 bits
int compressToFile_8_16(FILE *in, FILE *out, int bytes, key_type *keys) {

    unsigned short x = 0;
    char *buff = calloc( 100, sizeof( *buff ) );
    unsigned char y = 0;
    char* tmp;//

    while( fread(&x, 1, bytes, in ) == bytes){
		if(bytes == 2)
			x = ntohs(x);    //zamienia bajty w shortcie kolejnoscia
        	tmp =KeyToCode(keys[x]);//
		strcat(buff,tmp);//
		free(tmp);
		while( buff[7] != '\0' ){

            for(int i=7; i>=0; i--) {
                if( buff[i] == '1')
                    y |= (1 << (7 - i));
            }

            fwrite(&y, 1, 1, out);
            y=0;

            for(int i=0; i<92; i++)
                buff[i] = buff[i+8];
        } 
	}
	int i = 0;
	while( buff[i] != '\0' )
		i++;
	
	for(int j=i; j>=0; j--) {
        if( buff[j] == '1')
             y |= (1 << (7 - j));
    }
	if(i!=0)fwrite(&y, 1, 1, out);

    free(buff);
	return i;
}

//12 bits
int compressToFile_12(FILE *in, FILE *out, key_type *keys) {
   
    unsigned short x1 = 0;
	unsigned short x2 = 0;
	unsigned char tmp = 0;
	int check = 3;
    char *buff = calloc( 100, sizeof( *buff ) );
    unsigned char y = 0;
    char* keyGot;
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

        if(check >= 2){
           // strcat(buff, KeyToCode( keys[x1] ) );
		keyGot = KeyToCode(keys[x1]);
		strcat(buff,keyGot);
		free(keyGot);	
		}
        if(check == 3){
           // strcat(buff, KeyToCode( keys[x2] ) );
		keyGot = KeyToCode(keys[x2]);
		strcat(buff,keyGot);
		free(keyGot);
		}
        while( buff[7] != '\0' ){

            for(int i=7; i>=0; i--) {
                if( buff[i] == '1')
                    y |= 1 << (7 - i);
            }

            fwrite(&y, 1, 1, out);
            y=0;

            for(int i=0; i<92; i++)
                buff[i] = buff[i+8];
        }
	}

	int i = 0;
	while( buff[i] != '\0' )
		i++;

	for(int j=i; j>=0; j--) {
        if( buff[j] == '1')
             y |= (1 << (7 - j));
    }
	if(i!=0) fwrite(&y, 1, 1, out);

    free(buff);
	return i;
}
