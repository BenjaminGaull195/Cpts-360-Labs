
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>



typedef struct cmd {
	char cmd_line[256];
}Cmd;

typedef struct node {
	Cmd data;
	struct node *next;
} NODE;



//global variables

NODE * pipe_stack = NULL;
char *envp[], *paths[32] = { NULL };


int main(int argc, char *argv[], char *env[]) {
	char line[256] = { "\0" }, *arg[32] = { {"\0"} }, buf[256] = { "\0" }, *buf2;
	//char temp[3];
	int isPiped = 0;// isIRedirect = 0, isORedirect = 0, isAppend = 0;
	int count;
	NODE *p;

	
	//print and parse PATH environment variable
	//strcpy(path, getenv("PATH"));
	printf("%s", env[]);

	//loop through shell
	while (1) {
		//get input
		printf("mysh $ "); 
		fgets(line, 256, stdin); 
		line[strlen(line) - 1] = 0; 


		//parse command string; build stack for I/O redirection/pipes
		//sscanf(line, "%s %[^\n|]", arg[0], buf);
		count = 0;
		buf2 = strtok(line, " ");
		p = (NODE *)malloc(sizeof(NODE));
		while (buf2) {
			if (!strcmp(buf2, "|")) {
				push_stack(p);
				p = (NODE *)malloc(sizeof(NODE));
			}
			else {
				strcat(p->data.cmd_line, " ");
				strcat(p->data.cmd_line, buf2);
				strcat(p->data.cmd_line, " ");
			}
			
			buf2 = strtok(NULL, " ");
		}


		//execute command; build pipe; redirect output
		if (!strcmp(arg[0], "cd")) {
			if (strcmp(arg[0], "")) {
				chdir(arg[1]);
			}
			else {
				chdir(HOME);
			}
		}
		else if (!strcmp(arg[0], "exit")) {
			exit(0);
		}
		else if (isPiped) {	//pipe
			buildPipe(env);
		}
		else {

			if (!fork()) {
				execve(arg, env);
			}
		}
		isPiped = 0;


		//wait for command to finish execution:
		wait();
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

		//parse command; handle io redirect
		s = strtok(temp.cmd_line, " ");
		while (s) {
			if (s[0] == '>') {			//output redirect
				close(1);
				if (s[1] == '>') {		//append output redirect
					s = strtok(NULL, " ");
					open(s, O_APPEND, 0644);
				}
				else {
					s = strtok(NULL, " ");
					open(s, O_WRONLY, 0644);
				}
			}
			else if (s[0] == '<') {		// input redirect
				close(0);
				s = strtok(NULL, " ");
				open(s, O_RDONLY, 0644);
			}
			else {
				strcat(buf, s);
			}
			s = strtok(NULL, " ");
		}




		//execute command
		for (count = 0; count < 32; ++count) {
			if (paths[count] != "\0") {

				execve(paths[count], temp.cmd_line, envp);
			}
		}
	}

}








