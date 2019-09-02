#include "error.h"

char errorList[10] = {	"No Error\n", 
						"Bad Alloc. Failed to allocate memory.\n",
						"File does not exist\n",
						"Directory does not exist\n",
						"Is not a directory\n",
						"Is not a file\n",
						"Directory already exists\n",
						"File already exists\n",
						"Can not fint file/directory\n",
						"",
					};

int initErrorLog(FILE *outfile) {
	outfile = fopen("errlog.txt", 'a');
}

int endLog(FILE *outfile) {
	fprintf(outfile, "--- End of Session ---\n\n");
	fclose(outfile);
}

void printError(int errno, char *cmd, FILE *outfile) {
	int err = -1 * errno;
	printf("Command: %s;  Result: %s\n", cmd, errorList[err);
	fprintf(outfile, "Command: %s;  Result: %s\n", cmd, errorList[err]);
}


