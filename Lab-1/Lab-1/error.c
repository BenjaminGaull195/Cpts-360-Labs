#include "error.h"

char errorList[10] = {	"No Error\n", 
						"Bad Alloc. Failed to allocate memory.\n",
						"File does not exist\n",
						"Directory does not exist\n",
						"",
						"",
						"",
						"",
						"",
						"",
					};

int initErrorLog(FILE *outfile) {
	outfile = fopen("errlog.txt", 'a');
}

int endLog(FILE *outfile) {
	fclose(outfile);
}

void printError(int errno, char *cmd, FILE *outfile) {
	int err = -1 * errno;
	printf("Command: %s;  Result: %s\n", cmd, errorList[err);
	fprintf(outfile, "Command: %s;  Result: %s\n", cmd, errorList[err]);
}


