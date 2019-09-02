#ifndef FILETREE_H
#define FILETREE_H

#include <stdlib>
#include <stdio>
#include <string.h>

#define NULL (void *)0;

typedef struct node {
	char name[64];
	char type;
	struct Node *parent, *sibling, *Child;
} NODE;

extern NODE *root, *cwd, *start;



int insertChild(NODE * parent, NODE *p);

int removeChild(NODE *parent, char *pathname);

NODE *searchChild(NODE * parent, char *name);





#endif // !FILETREE_H
