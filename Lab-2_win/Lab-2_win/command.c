#include "command.h"


char *commands[15] = {"mkdir", "rmdir", "cd", "ls", "pwd", "creat", "rm", "save", "reload", "menu", "quit", "\0"};

cmd_ptr * cmd[15] = { &mkdir, &rmdir, &cd, &ls, &pwd, &creat, &rm, &save, &reload, &menu, &quit };
//char dname[64], bname[64];



int run_cmd(cmd_ptr cmd, char *temp) {
	
	return cmd(temp);
	

}


int findcmd(char *p) {
	int i = 0;
	while (commands[i]) {
		if (!strcmp(p, commands[i])) {
			return i;
		}
		++i;
	}
}



int initialize() {
	NODE *temp = malloc((NODE*)sizeof(NODE));
	if (!temp) {
		return -1;
	}
	root = temp;
	root->parent = root;
	root->sibling = root;
	root->child = NULL;
	root->type = 'D';
	strcpy(root->name, "/");
	cwd = root;
	return 0;
	//initErrorLog(logfile);
}




//mkdir pathname
int mkdir(char *pathname) {
	NODE *p, *q;
	int count = 0;
	char * s;
	printf("mkdir : dirname = %s\n", pathname);


	if (!strcmp(pathname, "/") || !strcmp(pathname, ".") || !strcmp(pathname, "..")) {
		printf("can't mkdir with %s\n", pathname);
		return -1;
	}
	if (pathname[0] == '/') {
		start = root;
	}
	else {
		start = cwd;
	}
	//break pathname into base name and dir name
	dbname(pathname);
	//printf("%s\n", dname);
	//printf("%s\n", bname);

	//tokenize 
	s = strtok(dname, "/");  // first call to strtok()
	/*if (dname[0] == '/'|| dname[0] == '\0') {
		s = strtok(0, "/");
	}*/

	if (strcmp(dname, "")) {
		
		while (s) {
			//++count;
			//printf(“%s “, s);
			
			q = searchChild(start, s, 'D');
			if (q && q->type == 'D') {
				start = q;
			}
			else if (!q && (dname[0] == '\0' || dname[0] == '.')) {
				printf("Error: Path not found\n");
				return -1;
			}
			else if (strcmp(q->type, "D")) {
				printf("Error: Pathname containes a file\n");
				return -1;
			}
			s = strtok(0, "/");  // call strtok() until it returns NULL


		}
	}
	
	
	q = searchChild(start, bname, 'D');
	if (!q) {
		p = (NODE *)malloc(sizeof(NODE));;
		p->type = 'D';
		p->child = NULL;
		p->sibling = NULL;
		strcpy(p->name, bname);
		insertChild(start, p);
		printf("makedir %s OK\n", pathname);
		return -1;

	}
	printf("Dir = %s already exists\n", pathname);
	return -1;


	//printf("check if %s already exists", pathname);
	//p = searchChild(start, pathname);
	//if (p) {
	//	printError(-7, "creat", logfile);
	//	return -1;
	//}

	//printf("ready to creat %s\n", pathname);
	//if (!p) {
	//	printError(-1, "creat", logfile);
	//	return -1
	//}

	
}

//rmdir pathname
int rmdir(char *pathname) {
	NODE *p, *q;
	int count = 0;
	char * s;
	printf("rmdir : dirname = %s\n", pathname);


	if (!strcmp(pathname, "/") || !strcmp(pathname, ".") || !strcmp(pathname, "..")) {
		printf("can't rmdir with %s\n", pathname);
		return -1;
	}
	if (pathname[0] == '/') {
		start = root;
	}
	else {
		start = cwd;
	}
	//break pathname into base name and dir name
	dbname(pathname);

	//tokenize 
	s = strtok(dname, "/");  // first call to strtok()

	if (!strcmp(dname, "")) {
		while (s) {
			q = searchChild(start, s, 'D');
			if (q && q->type == 'D') {
				start = q;
			}
			else if (!q) {
				printf("Error: Path not found\n");
				return -1;
			}
			else if (strcmp(q->type, "D")) {
				printf("Error: Pathname containes a file\n");
				return -1;
			}
			s = strtok(0, "/");  // call strtok() until it returns NULL

		}
	}

	p = q = start->child;
	if (!p) {
		printf("Error: directory not found\n");
	}
	while (p) {
		if (strcmp(p->name, bname) == 0) {
			q->sibling = p->sibling;
			free(p);
			printf("rmdir %s OK\n", pathname);
			return 0;
		}
		q = p;
		p = p->sibling;
	}
	printf("Error: Failed to find %s\n", pathname);
	return -1;
}

//cd <pathname>
int cd(char *pathname) {
	//!strcmp(pathname, ".") || !strcmp(pathname, "..")
	char *s;
	NODE *p;

	if (pathname[0] == '/') {
		start = root;
	}
	else {
		start = cwd;
	}
	p = start;
	s = strtok(pathname, "/");
	while (s) {
		if (!strcmp(pathname, "..")) {
			p = start->parent;
		}
		else if (!strcmp(pathname, ".")) {
			p = start;
		}
		else {
			p = searchChild(p, pathname, "D");

		}
		s = strtok(0, "/");
	}
	cwd = p;
	printf("cd %s OK\n", pathname);
	return 0;

}

//ls <pathname>
int ls(char *pathname) {
	NODE *p;
	char *s;
	//printf("%s\n", pathname);
	if (pathname[0] != '\0') {

		if (pathname[0] == '/') {
			start = root;
		}
		else {
			start = cwd;
		}


		s = strtok(pathname, "/");
		while (s) {
			start = searchChild(start, s, 'D');
			s = strtok(0, "/");
		}
		if (start) {
			p = start->child;
			printf("Contents of %s:\n", pathname);
			while (p) {
				printf("\t%c %s\n", p->type, p->name);
				p = p->sibling;
			}
			printf("ls %s OK\n", pathname);
			return 0;
		}
		printf("Error: %s not found", pathname);
		return -1;
	}
	else {
		p = start->child;
		printf("Contents of %s:\n", cwd->name);
		while (p) {
			printf("\t%c %s\n", p->type, p->name);
			p = p->sibling;
		}
		printf("ls %s OK\n", pathname);
		return 0;
	}

	/*NODE *p = cwd->child;
	printf("CWD contents: \n");
	while (p) {
		printf("\t%c %s\n", p->type, p->name);
		p = p->sibling;
	}*/
}

//pwd
int pwd(char *pathname) {
	printPath(cwd);
	putchar('\n');
	printf("pwd OK\n");
	return 0;
}

//creat <filename>
int creat(char *pathname) {
	NODE *p, *q;
	int count = 0;
	char * s;
	printf("creat : dirname = %s\n", pathname);


	if (!strcmp(pathname, "/") || !strcmp(pathname, ".") || !strcmp(pathname, "..")) {
		printf("can't creat with %s\n", pathname);
		return -1;
	}
	if (pathname[0] == '/') {
		start = root;
	}
	else {
		start = cwd;
	}
	//break pathname into base name and dir name
	dbname(pathname);
	//printf("%s\n", dname);
	//printf("%s\n", bname);

	//tokenize 
	s = strtok(dname, "/");  // first call to strtok()
	/*if (dname[0] == '/'|| dname[0] == '\0') {
		s = strtok(0, "/");
	}*/

	if (strcmp(dname, "")) {

		while (s) {
			//++count;
			//printf(“%s “, s);

			q = searchChild(start, s, 'F');
			if (q && q->type == 'F') {
				start = q;
			}
			else if (!q && (dname[0] == '\0' || dname[0] == '.')) {
				printf("Error: Path not found\n");
				return -1;
			}
			else if (strcmp(q->type, "D")) {
				printf("Error: Pathname containes a file\n");
				return -1;
			}
			s = strtok(0, "/");  // call strtok() until it returns NULL


		}
	}


	q = searchChild(start, bname, 'F');
	if (!q) {
		p = (NODE *)malloc(sizeof(NODE));;
		p->type = 'F';
		p->child = NULL;
		p->sibling = NULL;
		strcpy(p->name, bname);
		insertChild(start, p);
		printf("creat %s OK\n", pathname);
		return -1;

	}
	printf("File = %s already exists\n", pathname);
	return -1;
}

//rm <filename>
int rm(char *pathname) {
	NODE *p, *q;
	int count = 0;
	char * s;
	printf("rm : dirname = %s\n", pathname);


	if (!strcmp(pathname, "/") || !strcmp(pathname, ".") || !strcmp(pathname, "..")) {
		printf("can't rm with %s\n", pathname);
		return -1;
	}
	if (pathname[0] == '/') {
		start = root;
	}
	else {
		start = cwd;
	}
	//break pathname into base name and dir name
	dbname(pathname);

	//tokenize 
	s = strtok(dname, "/");  // first call to strtok()

	if (!strcmp(dname, "")) {
			while (s) {
				q = searchChild(start, s, 'F');
				if (q && q->type == 'F') {
					start = q;
				}
				else if (!q) {
					printf("Error: Path not found\n");
					return -1;
				}
				else if (strcmp(q->type, "F")) {
					printf("Error: Pathname containes a file\n");
					return -1;
				}
				s = strtok(0, "/");  // call strtok() until it returns NULL

			}
	}

	p = q = start->child;
	if (!p) {
		printf("Error: File not found\n");
	}
	while (p) {
		if (strcmp(p->name, bname) == 0) {
			q->sibling = p->sibling;
			free(p);
			printf("rm %s OK\n", pathname);
			return 0;
		}
		q = p;
		p = p->sibling;
	}
	printf("Error: Failed to find %s\n", pathname);
	return -1;
}

//save <filename>
int save(char *filename) {
	FILE *outfile;
	char temp[64] = { "" };
	strcpy(temp, strcat(filename, ".txt"));
	outfile = fopen(temp, "w");
	if 


	saveTree(cwd, outfile);
	fclose(outfile);

}

int saveTree(NODE* parent, FILE *outfile) {
	fprintf(outfile, "%c %s", parent->type, savePath(parent->parent, outfile));
	putc('\n', outfile);
	saveTree(parent->child, outfile);
	saveTree(parent->sibling, outfile);
}

int savePath(NODE* path, FILE *outfile) {
	if (!strcmp(path->name, "/")) {
		savePath(path->parent, outfile);
		
	}
	if (path->name == "/") {
		fprintf(outfile, "/");
		return 0;
	}
	else {
		fprintf(outfile, "%s/", path->name);
		return 0;
	}
	return -1;
}

int printPath(NODE *path) {
	if (strcmp(path->name, "/")) {
		printPath(path->parent);
	}
	if (!strcmp(path->name, "/")) {
		printf("/");
		return 0;
	}
	else {
		printf("%s/", path->name);
		return 0;
	}
	
	return -1;
}



//reload <filename>
int reload(char *filename) {
	FILE *infile = NULL;
	infile - fopen(strcat(filename, ".txt"), "r");
	char type = '\0', pathname[128] = {""};
	char line[128];

	while (type != EOF) {
		fgets(line, 128, infile);  // get at most 128 chars from stdin
		line[strlen(line)-1] = 0; // kill \n at end of line
		sscanf(line, "%c %s", type, pathname);
		if (type == 'D') {
			mkdir(pathname);
			return 0;
		}
		else if (type == 'F') {
			creat(pathname);
			return 0;
		}
		else {
			printf("Error: unrecognized type");

		}

	}
	return -1;
}

//menu
int menu() {
	printf("Available Commands:\n");
	printf("\tmkdir [pathname]  : make new directory in CWD\n");
	printf("\trmdir [pathname]  : remove directory if empty\n");
	printf("\tcd [pathname]     : change CWD to pathname, or root if '/'\n");
	printf("\tls [pathname]     : list contents of pathname or CWD\n");
	printf("\tpwd               : print absolute pathanem of CWD\n");
	printf("\tcreat [pathname]  : create a file\n");
	printf("\trm [pathname]     : remove a file\n");
	printf("\tsave [filename]   : save filesystem to file\n");
	printf("\treload [filename] : load filesystem from file\n");
	printf("\tquit              : save and exit program\n");
}

//quit
//may remove
int quit() {
	save("savefile.txt");
	printf("Exit Program\n");
	//endLog(logfile);
	exit(0);
}

