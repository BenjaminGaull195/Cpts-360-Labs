#include "fileTree.h"

NODE *root, *cwd, *start;

int insertChild(NODE * parent, NODE *p) {
	NODE * q = parent->child;
	if (q == 0) {
		parent->child = p;
	}
	else {
		while (q->sibling) {
			q = q->sibling;
		}
		q->sibling = p;
		p->parent = parent;
	}
	return 0;

}

NODE *searchChild(NODE * parent, char *name, char type) {
	NODE *p = parent->child;
	printf("search for %s in parent DIR\n", name);
	if (p == NULL) {
		return 0;
	}
	while (p) {
		if (strcmp(p->name, name) == 0) {
			return p;
			p = p->sibling;
		}
	}
	return 0;
}

int removeChild(NODE *parent, char *pathname) {
	NODE *p, *q;

	p = q = parent->child;
	if (p == 0) {
		return -1;
	}
	while (p && strcpy(p->name, pathname)) {
		q = p;
		p = p->sibling;
	}
	if (p != 0) {
		q->sibling = p->sibling;
		return 0;
	}
	return -1;

}









