#include <stdlib.h>
#include <limits.h>
#include "node.h"

dynamicArray *makeDynamicArray( int s ) {
        dynamicArray *a = malloc( sizeof( *a ) );
        a->t = malloc( s*sizeof *(a->t) );
	for(int i=0; i<s; i++) {
		a->t[i].quantity = 0;
		a->t[i].upper = NULL;
		a->t[i].left = NULL;
		a->t[i].right = NULL;
	}
        a->size = s;
        a->n = 0;
        return a;
}

void doubleSize( dynamicArray *da ) {
        node_t *tmp = malloc( 2 * da->size * sizeof *(da->t) );
        for(int i = 0; i < da->n ; i++)
                tmp[i] = da->t[i];
	for(int i = da->n; i < 2 * da->size; i++) {
		tmp[i].quantity = 0;
		tmp[i].upper = NULL;
		tmp[i].left = NULL;
		tmp[i].right = NULL;
	}	
        free( da->t );
        da->t = tmp;
        da->size *= 2;
}

void add( dynamicArray *da, char x ) {
        if( da->n == da->size )
                doubleSize( da );
        da->t[da->n].value = x;
	da->t[da->n].quantity++;
	da->n++;
}

void combineNodes( dynamicArray *da, node_t min1, node_t min2 ) {
	if( da->n == da->size )
		doubleSize( da );
	da->t[da->n].value = '\0';
	da->t[da->n].quantity = min1.quantity + min2.quantity;
	da->t[da->n].left = &min1;
	da->t[da->n].right = &min2;
	min1.upper = min2.upper = &( da->t[da->n] );
	da->n++;
}

void makeTree( dynamicArray *da ) {
	node_t min1;
	node_t min2;
	//miejsce jakiejsc petli jeszcze sie zobaczy
	min1.quantity = INT_MAX-1;           
	min2.quantity = INT_MAX;

	//ta petle i WARUNKI trzeba mocno potestowac - wrazliwy punkt programu
	for(int i = 0; i < da->n; i++) { 
		if( (da->t[i].upper == NULL) && (da->t[i].quantity != 0) ) {   
			if( (da->t[i].quantity < min2.quantity) && (da->t[i].quantity > min1.quantity) ) {
				min2.quantity = da->t[i].quantity;
			}
			else if( (da->t[i].quantity < min2.quantity) && (da->t[i].quantity < min1.quantity) ) {
				min2.quantity = min1.quantity;
				min1.quantity = da->t[i].quantity;
			}
		}	
	}
	printf("min1: %d	min2: %d\n", min1.quantity, min2.quantity );
}

			
