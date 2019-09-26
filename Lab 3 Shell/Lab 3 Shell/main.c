
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#ifdef _WIN32 || _WIN64
#include <io.h>
#include <process.h>
#define _CRT_SECURE_NO_WARNINGS
//#pragma warning(disable : 4996)
#else
#include <unistd.h>
#endif // _WIN32 || _WIN64



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
	char *appendBuf;
	int isPiped = 0, isIRedirect = 0;//, isORedirect = 0, isAppend = 0;
	int count, pid, status, fd;
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
		printf("!! Debug Help !!\n");
		while (buf2) {	//push cmd to stack for processing
			if (!strcmp(buf2, "|")) {
				push_stack(p);
				p = (NODE *)malloc(sizeof(NODE));
				isPiped = 1;
			}
			else {	//add str to cmd argv
				printf("%s\n", buf2);
				
				p->data.cmd_line[count] = buf2;
				++count;
			}
			
			buf2 = strtok(NULL, " ");
		}
		push_stack(p);

		temp = peek_stack();

		//execute command; build pipe; redirect output
		if (!strcmp(temp.cmd_line[0], "cd")) {
			if (strcmp(temp.cmd_line[1], "")) {
				chdir(temp.cmd_line[1]);
			}
			else {
				chdir(getenv("HOME"));
			}
		}
		else if (!strcmp(temp.cmd_line[0], "exit")) {	//handle exit
			exit(0);
		}
		else if (isPiped) {	//pipe
			pid = buildPipe(env);
			wait();
		}
		else {
			temp = peek_stack();
			pop_stack();
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

			if (pid) {
				//printf("Entered Parent\n");
				wait();
			}
			else {
				//printf("Entered child\n");
				//handle I/O Redirection
				//count = 0;

				count = 0;
				while (temp.cmd_line[count]) {

					if (!strcmp(temp.cmd_line[count], ">") || !strcmp(temp.cmd_line[count], ">>") || !strcmp(temp.cmd_line[count], "<")) {
						printf("Debug: Possible IO Redirection Present\n");
						//printf("%s\n", temp.cmd_line[count]);
					//	printf("%s\n", strcmp(temp.cmd_line[count], ">"));
					//	printf("%s\n", strcmp(temp.cmd_line[count], ">>"));
					//	printf("%s\n", strcmp(temp.cmd_line[count], "<"));

						//temp.cmd_line[count] = NULL;
						if (!strcmp(temp.cmd_line[count], ">")) {
							printf("Output Redirection\n");
							//close(1);
							//open(temp.cmd_line[count + 1], O_WRONLY, 0644);
							fd = open(temp.cmd_line[count + 1], O_WRONLY | O_CREAT);
							if (fd != -1) {
								printf("fd opened\n");
								if (dup2(1, fd) != -1) {
									printf("fd Opened\n");
								}
								else {
									printf("fd failed to open\n");
								}
							}
							else {
								printf("fd faled to open");
							}
						}
						else if (!strcmp(temp.cmd_line[count], ">>")) {
							printf("Outfut Append Redirection\n");
							//close(1);
							fd = open(temp.cmd_line[count + 1], O_APPEND | O_CREAT);
							if (fd != -1) {
								printf("fd opened\n");
								if (dup2(1, fd) != -1) {
									printf("fd Opened\n");
								}
								else {
									printf("fd failed to open\n");
								}
							}
							else {
								printf("fd faled to open");
							}

						}
						else if (!strcmp(temp.cmd_line[count], "<")) {
							printf("Input Redirection\n");
							//close(0);
							//open(temp.cmd_line[count + 1], O_RDONLY);
							fd = open(temp.cmd_line[count + 1], O_RDONLY);
							if (fd != -1) {
								printf("fd opened\n");
								if (dup2(0, fd) != -1) {
									printf("fd Opened\n");
								}
								else {
									printf("fd failed to open\n");
								}
							}
							else {
								printf("fd failed to open\n");
							}
						}


						while (temp.cmd_line[count]) {
							temp.cmd_line[count] = NULL;
							++count;
						}
					}
					printf("%s\n", temp.cmd_line[count]);
					++count;
				}


				//printf("Debug: ready to execute\n");
				for (count = 0; count < 32 && paths[count]; ++count) {
					if (paths[count] != NULL) {
						printf("Searching %s\n", paths[count]);

						strcpy(appendBuf, paths[count]);
						strcat(appendBuf, "/");
						strcat(appendBuf, temp.cmd_line[0]);
						execve(appendBuf, temp.cmd_line, env);
					}
				}
			}
			isIRedirect = isPiped = 0;
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
	int pid, count, isRedirect, isPiped;
	Cmd temp;
	int pd[2], fd;
	char buf[256], *s = 0, *appendBuf;

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
		return pid;

	}
	else {		//child
		//configure pipe recieve
		close(pd[1]);
		close(0);
		dup(pd[0]);

		count = 0;
		//parse command; handle io redirect
		
	
		//printf("Entered child\n");
		//handle I/O Redirection
		//count = 0;
		while (temp.cmd_line[count]) {

			if (!strcmp(temp.cmd_line[count], ">") || !strcmp(temp.cmd_line[count], ">>") || !strcmp(temp.cmd_line[count], "<")) {
				printf("Debug: Possible IO Redirection Present\n");
				//printf("%s\n", temp.cmd_line[count]);
			//	printf("%s\n", strcmp(temp.cmd_line[count], ">"));
			//	printf("%s\n", strcmp(temp.cmd_line[count], ">>"));
			//	printf("%s\n", strcmp(temp.cmd_line[count], "<"));

				//temp.cmd_line[count] = NULL;
				if (!strcmp(temp.cmd_line[count], ">")) {
					printf("Output Redirection\n");
					//close(1);
					//open(temp.cmd_line[count + 1], O_WRONLY, 0644);
					fd = open(temp.cmd_line[count + 1], O_WRONLY | O_CREAT);
					if (fd != -1) {
						printf("fd opened\n");
						if (dup2(1, fd) != -1) {
							printf("fd Opened\n");
						}
						else {
							printf("fd failed to open\n");
						}
					}
					else {
						printf("fd faled to open");
					}
				}
				else if (!strcmp(temp.cmd_line[count], ">>")) {
					printf("Outfut Append Redirection\n");
					//close(1);
					fd = open(temp.cmd_line[count + 1], O_APPEND | O_CREAT);
					if (fd != -1) {
						printf("fd opened\n");
						if (dup2(1, fd) != -1) {
							printf("fd Opened\n");
						}
						else {
							printf("fd failed to open\n");
						}
					}
					else {
						printf("fd faled to open");
					}

				}
				else if (!strcmp(temp.cmd_line[count], "<")) {
					printf("Input Redirection\n");
					//close(0);
					//open(temp.cmd_line[count + 1], O_RDONLY);
					fd = open(temp.cmd_line[count + 1], O_RDONLY);
					if (fd != -1) {
						printf("fd opened\n");
						if (dup2(0, fd) != -1) {
							printf("fd Opened\n");
						}
						else {
							printf("fd failed to open\n");
						}
					}
					else {
						printf("fd failed to open\n");
					}
				}


				while (temp.cmd_line[count]) {
					temp.cmd_line[count] = NULL;
					++count;
				}
			}
			printf("%s\n", temp.cmd_line[count]);
			++count;
		}


		//printf("Debug: ready to execute\n");
		for (count = 0; count < 32 && paths[count]; ++count) {
			if (paths[count] != NULL) {
				printf("Searching %s\n", paths[count]);

				strcpy(appendBuf, paths[count]);
				strcat(appendBuf, "/");
				strcat(appendBuf, temp.cmd_line[0]);
				execve(appendBuf, temp.cmd_line, envp);
			}
		}
		isRedirect = isPiped = 0;
	}

}








