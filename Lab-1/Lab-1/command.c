#include "command.h"


char *commands[15] = {"mkdir", "rmdir", "cd", "ls", "Pwd", "creat", "rm", "save", "reload", "menu", "quit", '\0'};

cmd_ptr * cmd[15] = { &mkdir, &rmdir, &cd, &ls, &pwd, &creat, &rm, &save, &reload, &menu, &quit };
char dname[64], bname[64];



int run_cmd(cmd_ptr cmd, char *temp) {
	
	if (temp[0] = '\0') {
		return cmd();
	}
	else {
		return cmd(temp);
	}

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

	//tokenize 
	s = strtok(dname, "/");  // first call to strtok()
	while (s) {
		//++count;
		//printf(“%s “, s);
		q = searchChild(start, s, 'D');
		if (q && !strcmp(q->type, "D")) {
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
	q = searchChild(start, bname, 'D');
	if (!q) {
		p = (NODE *)malloc(sizeof(NODE));;
		p->type = 'D';
		strcpy(p->name, pathname);
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
	while (s) {
		q = searchChild(start, s, 'D');
		if (q && !strcmp(q->type, "D")) {
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

}

//ls <pathname>
int ls(char *pathname) {
	NODE *p = cwd->child;
	printf("CWD contents: \n");
	while (p) {
		printf("\t%c %s\n", p->type, p->name);
		p = p->sibling;
	}
}

//pwd
int pwd() {
	

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

	//tokenize 
	s = strtok(dname, "/");  // first call to strtok()
	while (s) {
		++count;
		//printf(“%s “, s);
		s = strtok(0, "/");  // call strtok() until it returns NULL
		q = searchChild(start, s, 'F');
		if (q && !strcmp(q->type, "F")) {
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

	}
	q = searchChild(start, bname, 'F');
	if (!q) {
		p = (NODE *)malloc(sizeof(NODE));;
		p->type = 'F';
		strcpy(p->name, pathname);
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
	while (s) {
		q = searchChild(start, s, 'F');
		if (q && !strcmp(q->type, "F")) {
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
	outfile = fopen(strcat(filename, ".txt"), "w+");
	saveTree(cwd, outfile);
	fclose(outfile);

}

int saveTree(NODE* parent, FILE *outfile) {
	fprintf(outfile, "%c %s", parent->type, printPath(parent->parent, outfile));
	putc('\n', outfile);
	saveTree(parent->child, outfile);
	saveTree(parent->sibling, outfile);
}

int printPath(NODE* path, FILE *outfile) {
	if (path->name != "/") {
		printPath(path->parent, outfile);
		if (path->name == "/") {
			fprintf(outfile, "/");
			return 0;
		}
		else {
			fprintf(outfile, "%s/", path->name);
			return 0;
		}
	}
	return -1;
}



//reload <filename>
int reload(char *filename) {
	FILE *infile;
	infile - fopen(strcat(filename, ".txt"), "r");
	char type, *pathname;
	char line[128];

	while () {
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
	return 1;
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

