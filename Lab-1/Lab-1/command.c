#include "command.h"


char *commands[25] = {"mkdir", "rmdir", "cd", "ls", "Pwd", "creat", "rm", "save", "reload", "menu", "quit"};


int run_cmd(cmd_prt cmd, char *temp) {
	
	if (temp[0] = '\0') {
		return cmd();
	}
	else {
		return cmd(temp);
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
}




//mkdir pathname
int mkdir(char *pathname) {

}

//rmdir pathname
int rmdir(char *pathname) {

}

//cd <pathname>
int cd(char *pathname) {

}

//ls
int ls() {

}

//pwd
int pwd() {

}

//creat <filename>
int creat(char *filename) {

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

}

//quit
//may remove
int quit() {

}

