#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

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
	char appendBuf[128];
	int isPiped = 0, isIRedirect = 0; //isORedirect = 0, isAppend = 0;
	int count, pid, status;
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





    while(1) {

        printf("mysh: $ "); 
		fgets(line, 256, stdin); 
		line[strlen(line) - 1] = 0; 

		//parse command string; build stack for I/O redirection/pipes
		//sscanf(line, "%s %[^\n|]", arg[0], buf);
		
		buf2 = strtok(line, " ");
		count = 0;
		p = (NODE *)malloc(sizeof(NODE));
		//printf("!! Debug Help !!\n");
		while (buf2) {
			if (!strcmp(buf2, "|")) {
                //printf("Debug: push pipe to stack\n");
				push_stack(p);
				p = (NODE *)malloc(sizeof(NODE));
				isPiped = 1;
			}
			else {
				//printf("%s\n", buf2);
				//strcat(p->data.cmd_line, " ");
				//strcat(p->data.cmd_line, buf2);
				//strcat(p->data.cmd_line, " ");
				p->data.cmd_line[count] = buf2;
				++count;
			}
			//printf("Debug: get next token\n");
			buf2 = strtok(NULL, " ");
		}
        //printf("Debug: push to stack\n");
		push_stack(p);

        //printf("Debug: get command\n");
		temp = peek_stack();

        if (!strcmp(temp.cmd_line[0], "cd")) {			//hadnle cd
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
        else {											//handle single commands
			
            pid = fork();

            if(pid) {
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
						printf("Debug: IO Redirection Present\n");
						//temp.cmd_line[count] = NULL;
                        if (!strcmp(temp.cmd_line[count], ">")) {
							printf("Output Redirection\n");
                            //close(1);
                            //open(temp.cmd_line[count + 1], O_WRONLY, 0644);
							if (dup2(1, open(temp.cmd_line[count + 1], O_WRONLY, 0644)) != -1) {
								printf("fd Opened\n");
							}
							else {
								printf("fd failed to open\n");
							}
                        }
                        else if (!strcmp(temp.cmd_line[count], ">>")) {
							printf("Outfut Append Redirection\n");
                            //close(1);
							if (dup2(1, open(temp.cmd_line[count + 1], O_APPEND, 0644)) != -1) {
								printf("fd Opened\n");
							}
							else {
								printf("fd failed to open\n");
							}

                        }
                        else {
							printf("Input Redirection\n");
                            close(0);
                            open(temp.cmd_line[count + 1], O_RDONLY);
							if (dup2(0, open(temp.cmd_line[count + 1], _O_RDONLY)) != -1) {
								printf("fd Opened\n");
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

