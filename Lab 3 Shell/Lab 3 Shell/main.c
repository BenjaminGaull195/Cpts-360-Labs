
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


//structs

typedef struct cmd {
	char *cmd_line[32];
}Cmd;

typedef struct node {
	Cmd data;
	struct node *next;
} NODE;


//global variables

NODE * pipe_stack = NULL;
char *envptr, *paths[32] = { NULL }, *arg[32] = { NULL };


//function prototypes
int push_stack(NODE *p);

int pop_stack();

Cmd peek_stack();

int buildPipe(char **envp);


//main

int main(int argc, char *argv[], char *env[]) {
	char line[256] = { "\0" }, buf[256] = { "\0" }, *buf2;
	//char temp[3];
	int isPiped = 0;// isIRedirect = 0, isORedirect = 0, isAppend = 0;
	int count, pid;
	NODE *p;
	Cmd temp;


	
	//print and parse PATH environment variable
	//strcpy(path, getenv("PATH"));
	printf("!!! Init PATH Variable !!!\n\n");

	envptr = getenv("PATH");
	printf("PATH=%s", envptr);

	//parce PATH
	count = 0;
	buf2 = strtok(envptr, ":");
	while (buf2) {
		if (strcmp(buf2, "")) {
			paths[count] = buf2;
			++count;
		}
		buf2 = strtok(NULL, ":");
		if (!buf2) {
			paths[count] = NULL;
		}
	}
	printf("\n\nParced PATH Variable:\n");
	count = 0;
	while (paths[count]) {
		printf("%s\n", paths[count]);
		++count;
	}

	printf("\n!!! PATH Initialized !!!\n\n");

	//loop through shell
	while (1) {
		//get input

		
		printf("mysh: $ "); 
		fgets(line, 256, stdin); 
		line[strlen(line) - 1] = 0; 

		//parse command string; build stack for I/O redirection/pipes
		//sscanf(line, "%s %[^\n|]", arg[0], buf);
		
		buf2 = strtok(line, " ");
		count = 0;
		p = (NODE *)malloc(sizeof(NODE));
		printf("!! Debug Help !!\n")
		while (buf2) {
			if (!strcmp(buf2, "|")) {
				push_stack(p);
				p = (NODE *)malloc(sizeof(NODE));
				isPiped = 1;
			}
			else {
				printf("%s\n", buf2);
				//strcat(p->data.cmd_line, " ");
				//strcat(p->data.cmd_line, buf2);
				//strcat(p->data.cmd_line, " ");
				p->data.cmd_line[count] = buf2;
				++count;
			}
			
			buf2 = strtok(NULL, " ");
		}
		push_stack(p);

		temp = peek_stack();

		//execute command; build pipe; redirect output
		if (!strcmp(temp.cmd_line[0], "cd")) {
			if (strcmp(arg[0], "")) {
				chdir(arg[1]);
			}
			else {
				chdir(getenv("HOME"));
			}
		}
		else if (!strcmp(temp.cmd_line[0], "exit")) {
			exit(0);
		}
		else if (isPiped) {	//pipe
			buildPipe(env);
			wait();
		}
		else {
			temp = peek_stack();
		/*	for (count = 0; count < 32; ++count) {
				if (paths[count] != "\0") {
					execve(paths[count], temp.cmd_line, env);
				}
			}*/
			/*
			if (!fork()) {

				execve(arg, env);
			}
			*/

			pid = fork();
			if (pid) {		//parent
				//configure pipe send
				//close(pd[0]);
				//close(1);
				//dup(pd[1]);
				wait();
				return 0;

			}
			else {		//child
				//configure I/O redirect


				count = 0;
				//parse command; handle io redirect
				while (buf2) {
					buf2 = temp.cmd_line[count];

					if (buf2[0] == '>') {			//output redirect
						close(1);
						if (buf2[1] == '>') {		//append output redirect
							++count;
							open(buf2, O_APPEND, 0644);
						}
						else {
							++count;
							open(buf2, O_WRONLY, 0644);
						}
					}
					else if (buf2[0] == '<') {		// input redirect
						close(0);
						++count;
						open(buf2, O_RDONLY, 0644);
					}
					else {
						++count;
					}
					
				}
				temp.cmd_line[count] = NULL;
				
				//execute command
				for (count = 0; count < 32; ++count) {
					if (paths[count] != "\0") {
						//printf("Searching %s\n", paths[count]);
						execve(strcat(strcat(paths[count], "/"), temp.cmd_line[0]), temp.cmd_line, env);
					}
				}
			}





		}
		isPiped = 0;


		//wait for command to finish execution:
		//wait();
	}

}







int push_stack(NODE *p) {
	p->next = pipe_stack;
	pipe_stack = p;
	return 0;
}

int pop_stack() {
	NODE * temp = pipe_stack;
	pipe_stack = pipe_stack->next;
	free(temp);
	return 0;
}

Cmd peek_stack() {
	return pipe_stack->data;
}


int buildPipe(char **envp) {
	int pid, count;
	Cmd temp;
	int pd[2];
	char buf[256], *s;

	if (pipe_stack == NULL) {
		return 0;
	}
	else {
		buildPipe(envp);
	}

	//get stack item
	temp = peek_stack();
	pop_stack();

	//set up pipe
	pipe(pd);

	//fork
	pid = fork();
	if (pid) {		//parent
		//configure pipe send
		close(pd[0]);
		close(1);
		dup(pd[1]);
		return 0;

	}
	else {		//child
		//configure pipe recieve
		close(pd[1]);
		close(0);
		dup(pd[0]);

		count = 0;
		//parse command; handle io redirect
		
		while (s) {
			s = temp.cmd_line[count];

			if (s[0] == '>') {			//output redirect
				close(1);
				if (s[1] == '>') {		//append output redirect
					++count;
					open(s, O_APPEND, 0644);
				}
				else {
					++count;
					open(s, O_WRONLY, 0644);
				}
			}
			else if (s[0] == '<') {		// input redirect
				close(0);
				++count;
				open(s, O_RDONLY, 0644);
			}
			else {
				++count;
			}
		}
		temp.cmd_line[count] = NULL;




		//execute command
		for (count = 0; count < 32; ++count) {
			if (paths[count] != NULL) {
				printf("Searching %s\n", paths[count]);
			
				execve(strcat(strcat(paths[count],"/"),temp.cmd_line[0]), temp.cmd_line, envp);
			}
		}
	}

}








