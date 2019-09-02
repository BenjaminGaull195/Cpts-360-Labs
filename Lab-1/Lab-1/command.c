#include "command.h"
#include "util.h"


char *commands[15] = {"mkdir", "rmdir", "cd", "ls", "Pwd", "creat", "rm", "save", "reload", "menu", "quit", NULL};

extern cmd_ptr * cmd[15] = { &mkdir, &rmdir, &cd, &ls, &pwd, &creat, &rm, &save, &reload, &menu, &quit };


int run_cmd(cmd_prt cmd, char *temp) {
	
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
	root->Child = NULL;
	root->type = 'D';
	strcpy(root->name, "/");
	cwd = root;
	return 0;
	initErrorLog(logfile);
}




//mkdir pathname
int mkdir(char *pathname) {
	NODE *p = (NODE *)malloc(sizeof(NODE));
	printf("mkdir : dirname = %s\n", pathame);

	if (strcmp(pathname, "/") == 0) {
		printf("can't mkdir with /\n");
		return -1;
	}
	if (pathname[0] == '/') {
		start = root;
	}
	else {
		start = cwd;
	}

	printf("check if %s already exists", pathname);
	p = searchChild(start, pathname);
	if (p) {
		printError(-7, "creat", logfile);
		return -1;
	}

	printf("ready to creat %s\n", pathname);
	if (!p) {
		printError(-1, "creat", logfile);
		return -1
	}


	p->type = 'D';
	strcpy(p->name, pathname);
	insertChild(start, p);
	printf("makedir %s OK\n", pathname);
	return 0;
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
int creat(char *filename) {
	NODE *p = (NODE *)malloc(sizeof(NODE));
	printf("creat : filename = %s\n", filename);

	if (filename[0] == '/') {
		start = root;
	}
	else {
		start = cwd;
	}

	printf("check if %s already exists", filename);
	p = searchChild(start, filename);
	if (p) {
		printError(-7, "creat", logfile);
		return -1;
	}

	printf("ready to creat %s\n", filename);
	if (!p) {
		printError(-1, "creat", logfile);
		return -1
	}


	p->type = 'F';
	strcpy(p->name, filename);
	insertChild(start, p);
	printf("makedir %s OK\n", filename);
	return 0;
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

