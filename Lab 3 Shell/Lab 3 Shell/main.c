
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>



typedef struct cmd {
	char cmd[32];
	char arg[128];
}Cmd;

typedef struct node {
	Cmd data;
	struct node *next;
} NODE;



//global variables
char path[256] = { "\0" }, *paths[32] = { {"\0"} };

NODE * cmd_stack = NULL;


int main(int argc, char *argv[], char *env[]) {
	char line[256] = { "\0" }, command[32] = { "\0" }, arg[224] = { "\0" }, buf[256] = { "\0" }, *buf2;
	char temp[3];

	strcpy(path, getenv("PATH"));

	printf("%s", path);

	//loop through shell
	while () {
		//get input


		//parse command string; build stack for I/O redirection/pipes
		
		


		//execute command; build pipe; redirect output
		if (temp[1] == '|') {
			buildPipe();
		}
		else if (temp[1] == '>' || temp [1] == '<') {
			if (temp[1] == '>') {

			}
			else {

			}
		}
		else if (!strcmp(temp, ">>") || !strcmp(temp, "<<")) {
			if (!strcmp(temp, ">>")) {

			}
			else {

			}
		}
		else {

		}



		//wait for command to finish execution:
		wait();
	}

}







int push_stack(NODE *p) {
	p->next = cmd_stack;
	cmd_stack = p;
	return 0;
}

int pop_stack() {
	NODE * temp = cmd_stack;
	cmd_stack = cmd_stack->next;
	free(temp);
	return 0;
}

Cmd peek_stack() {
	return cmd_stack->data;
}


int buildPipe() {
	int pid, count;
	Cmd temp;
	int fd[2];

	if (cmd_stack == NULL) {
		return 0;
	}

	//get stack item
	temp = peek_stack();
	pop_stack();

	//set up pipe
	

	//fork
	pid = fork();
	if (pid) {		//parent
		//configure pipe send
		close(1);
		open(fd[1]);

	}
	else {		//child
		//configure pipe recieve
		close(0);
		open(fd[0]);

		//execute command
		for (count = 0; count < 32; ++count) {
			if (paths[count] != "\0") {

				execve(paths[count], temp.cmd, temp.arg);
			}
		}
	}

}








