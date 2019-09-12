#ifndef FILETREE_H
#define FILETREE_H

#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NULL (void *)0;

typedef struct node {
	char name[64];
	char type;
	struct Node *parent, *sibling, *child;
} NODE;

extern NODE *root, *cwd, *start;



int insertChild(NODE * parent, NODE *p);

int removeChild(NODE *parent, char *pathname);

NODE *searchChild(NODE * parent, char *name, char type);





#endif // !FILETREE_H
