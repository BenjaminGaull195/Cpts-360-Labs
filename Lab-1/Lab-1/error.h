#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>

extern char errorList[10];

int initErrorLog(FILE *outfile);

int endLog(FILE *outfile);

void printError(int errno, char *cmd, FILE *outfile);




#endif // !ERROR_H