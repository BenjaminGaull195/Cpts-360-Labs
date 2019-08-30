#include <stdio.h>             // for I/O
#include <stdlib.h>            // for I/O
#include <libgen.h>            // for dirname()/basename()
#include <string.h>

typedef struct node {
	char  name[64];       // node's name string
	char  type;
	struct node *child, *sibling, *parent;
}NODE;


NODE *root, *cwd, *start;
char line[128];
char command[16], pathname[64];

char *cmd[] = { "mkdir", "ls", "quit", 0 };

int findCmd(char *command)
{
	int i = 0;
	while (cmd[i]) {
		if (strcmp(command, cmd[i]) == 0)
			return i;
		i++;
	}
	return -1;
}

NODE *search_child(NODE *parent, char *name)
{
	NODE *p = parent->child;
	printf("search for %s in parent DIR\n", name);
	if (p == 0)
		return 0;
	while (p) {
		if (strcmp(p->name, name) == 0)
			return p;
		p = p->sibling;
	}
	return 0;
}

int insert_child(NODE *parent, NODE *q)
{
	NODE *p = parent->child;
	printf("insert NODE %s into parent child list\n", q->name);
	if (p == 0)
		parent->child = q;
	else {
		while (p->sibling)
			p = p->sibling;
		p->sibling = q;
	}
	q->sibling = 0;
}

int mkdir(char *name)
{
	NODE *p, *q;
	printf("mkdir: name=%s\n", name);

	if (strcmp(name, "/") == 0) {
		printf("can't mkdir with /\n");
		return -1;
	}
	if (name[0] == '/')
		start = root;
	else
		start = cwd;

	printf("check whether %s already exists\n", name);
	p = search_child(start, name);
	if (p) {
		printf("name %s already exists, mkdir FAILED\n", name);
		return -1;
	}

	printf("ready to mkdir %s\n", name);
	q = (NODE *)malloc(sizeof(NODE));
	q->type = 'D';
	strcpy(q->name, name);
	insert_child(start, q);
	printf("mkdir %s OK\n", name);
	return 0;
}

int ls()
{
	NODE *p = cwd->child;
	printf("cwd contents = ");
	while (p) {
		printf("[%c %s] ", p->type, p->name);
		p = p->sibling;
	}
	printf("\n");
}

int quit()
{
	printf("Program exit\n");
	exit(0);
}

int initialize()
{
	root = (NODE *)malloc(sizeof(NODE));
	strcpy(root->name, "/");
	root->parent = root;
	root->sibling = 0;
	root->child = 0;
	root->type = 'D';
	cwd = root;
	printf("Root initialized OK\n");
}

int main()
{
	int index;
	initialize();

	printf("NOTE: commands = mkdir|ls|quit\n");

	while (1) {
		printf("Enter command line : ");
		fgets(line, 128, stdin);
		line[strlen(line) - 1] = 0;

		command[0] = pathname[0] = 0;
		sscanf(line, "%s %s", command, pathname);
		printf("command=%s pathname=%s\n", command, pathname);

		if (command[0] == 0)
			continue;

		index = findCmd(command);

		switch (index) {
		case 0: mkdir(pathname); break;
		case 1: ls(); break;
		case 2: quit();
		}
	}
}

