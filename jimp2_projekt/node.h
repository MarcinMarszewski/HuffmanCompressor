#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	char value;
	int quantity;
	struct node *left;
	struct node *right;
	struct node *upper;
} node_t;

//dynamic array of nodes
typedef struct dynamicArray {     
	node_t **t;               //array of pointers to nodes
	int size;                 //size of array
	int n;                    //amount of nodes
} dynamicArray;

dynamicArray *makeDynamicArray( int );

void doubleSize( dynamicArray * );

void add( dynamicArray *, char );

void combineNodes( dynamicArray *, node_t *, node_t * );

void makeTree( dynamicArray * ); 
