#ifndef TREEWRITER
#define TREEWRITER

#include "fileWriter.h"
#include "fileReader.h"
#include "node.h"
#include <stdlib.h>

int WriteTree(node_t* head);

int ReadTree(node_t* head);

void SetWordSize(int n);

#endif // TREEWRITER
