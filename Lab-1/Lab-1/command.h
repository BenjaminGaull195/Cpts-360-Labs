#ifndef COMMAND_H
#define COMMAND_H

#include "fileTree.h"
#include "error.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>





extern char *commands[25];			//array of command strings
extern char dname[64], bname[64];


typedef int(*cmd_ptr)(char*);		//function pointer for string parameter
typedef int(*cmd_ptr)();			//function pointer for no parameter

extern cmd_ptr * cmd[];				//array of function pointers

//function to execute commands
int run_cmd(cmd_ptr cmd, char *temp);	

//to initialize when first launched
int initialize();

int findcmd(char *p);

//mkdir pathname
int mkdir(char *pathname);

//rmdir pathname
int rmdir(char *pathname);

//cd <pathname>
int cd(char *pathname);

//ls
int ls(char *pathname);

//pwd
int pwd();

//creat <filename>
int creat(char *pathname);

//rm <filename>
int rm(char *pathname);

//save <filename>
int save(char *filename);

int saveTree(NODE* parent, FILE *outfile);

int printPath(NODE* path, FILE *outfile);

//reload <filename>
int reload(char *filename);

//menu
int menu();

//quit
//may remove
int quit();





#endif // !COMMAND_H
