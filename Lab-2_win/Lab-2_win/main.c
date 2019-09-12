//#include "error.h"
#include "util.h"
#include "fileTree.h"
#include "command.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char line[128];						// user input command line 
char command[16], pathname[64];		//command and pathname strings
			//dirname and basename string holders
//FILE *logfile = NULL;

int main(int argc, char *argv[], char *env[]) {
	initialize();

	while (1) {
		printf("Enter \"menu\" for a list of commands.\n");
		printf("Enter a Command: ");
		fgets(line, 128, stdin);
		line[strlen(line) - 1] = 0;
		sscanf(line, "%s %s", command, pathname);
		run_cmd(cmd[findcmd(command)], pathname);

		//reset pathname
		pathname[0] = '\0';

	}



	return 0;
}