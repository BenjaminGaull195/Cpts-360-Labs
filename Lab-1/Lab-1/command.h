#ifndef COMMAND_H
#define COMMAND_H

#include "fileTree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


extern char *commands[25];

typedef int(*cmd_prt)(char*);
typedef int(*cmd_prt)();

//
int run_cmd(cmd_prt cmd, char *temp);

//to initialize when first launched
int initialize();



//mkdir pathname
int mkdir(char *pathname);

//rmdir pathname
int rmdir(char *pathname);

//cd <pathname>
int cd(char *pathname);

//ls
int ls();

//pwd
int pwd();

//creat <filename>
int creat(char *filename);

//rm <filename>
int rm(char *filename);

//save <filename>
int save(char *filename);

//reload <filename>
int reload(char *filename);

//menu
int menu();

//quit
//may remove
int quit();





#endif // !COMMAND_H
