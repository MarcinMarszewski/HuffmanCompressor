#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "node.h"

dynamicArray *makeDynamicArray( int s ) {
        dynamicArray *a = malloc( sizeof( *a ) );
        a->t = malloc( s*sizeof **(a->t) );
	for(int i=0; i<s; i++) {
		a->t[i] = malloc( sizeof *( a->t[i] ) );
		a->t[i]->value = '\0';
		a->t[i]->quantity = 0;
		a->t[i]->upper = NULL;
		a->t[i]->left = NULL;
		a->t[i]->right = NULL;
	}
        a->size = s;
        a->n = 0;
        return a;
}

void doubleSize( dynamicArray *da ) {                             
        node_t **tmp = malloc( 2 * da->size * sizeof **(da->t) );
	for(int i = 0; i < da->n; i++ )
		tmp[i] = da->t[i];
	for(int i = da->n; i < 2 * da->size; i++) {
		tmp[i] = malloc( sizeof *( tmp[i] ) );
		tmp[i]->value = '\0';
		tmp[i]->quantity = 0;
		tmp[i]->upper = NULL;
		tmp[i]->left = NULL;
		tmp[i]->right = NULL;
	}
	free( da->t );
	da->t = tmp;
        da->size *= 2;
}

void add( dynamicArray *da, unsigned short x ) {
        if( da->n == da->size )
                doubleSize( da );
        da->t[da->n]->value = x;
	da->t[da->n]->quantity++;
	da->n++;
}

void combineNodes( dynamicArray *da, node_t *min1, node_t *min2 ) {
	if( da->n == da->size )
		doubleSize( da );
	da->t[da->n]->quantity = min1->quantity + min2->quantity;
	da->t[da->n]->left = min1;
	da->t[da->n]->right = min2;
	min1->upper = min2->upper = da->t[da->n];
	da->n++;
}

void makeTree( dynamicArray *da ) { 
	node_t tmp1;
	node_t tmp2;
	int l=3;
	while( l>2 ) {
		l=0;
		node_t *min1 = &tmp1;
       		node_t *min2 = &tmp2;
		tmp1.quantity = INT_MAX-1;          
		tmp2.quantity = INT_MAX;
		for(int i = 0; i < da->n; i++) {
			if( (da->t[i]->upper == NULL) && (da->t[i]->quantity != 0) ) {
			     	l++;	
				if( (da->t[i]->quantity < min2->quantity) && (da->t[i]->quantity >= min1->quantity) ) {
					min2 = da->t[i];
				}
				else if( (da->t[i]->quantity < min2->quantity) && (da->t[i]->quantity < min1->quantity) ) {
					min2 = min1;
					min1 = da->t[i];
				}
			}	
		}
	combineNodes( da, min1, min2 ); 
	}
}

			
