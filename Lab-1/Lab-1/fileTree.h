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

int removeChild();

int insertSibling(NODE * parent);

int removeSibling(NODE * parent);

NODE *searchChild(NODE * parent, char *name);

//may remove later
NODE *searchSibling(NODE * Parent, char *name);



#endif // !FILETREE_H
