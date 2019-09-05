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
	initErrorLog(logfile);
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
		++count;
		//printf(“%s “, s);
		s = strtok(0, "/");  // call strtok() until it returns NULL
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
	char *dirname_tokens[25];
	int count = 0;
	NODE * p;

	dbname(pathname);

	if (strcmp(dname, "/") == 0) {
		printf("can't rmdir with /\n");
		return -1;
	}
	if (dname[0] == '/') {
		start = root;
	}
	else {
		start = cwd;
	}

	tokenize(dname, dirname_tokens);
	while (dirname_tokens[count]) {
		p = searchChild(start, dirname_token);
		
		
		if (!p) {
			printf("pathanme %s not found\n", pathname);
			return -1;
		}
		start = start->child;
		++count;
	}
	if (!p->child) {
		printf("directory is empty\n")
			//start = start->parent;
			removeChild(start, bname);
		
	}




}

//cd <pathname>
int cd(char *pathname) {

}

//ls
int ls() {
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
int rm(char *filename) {

}

//save <filename>
int save(char *filename) {

}

//reload <filename>
int reload(char *filename) {

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
	endLog(logfile);
	exit(0);
}

