#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <unistd.h>
#include <netdb.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <arpa/inet.h>


#define MAX   256
#define PORT 1234
#define BLKSIZE 4096

char line[MAX], ans[MAX];
int n;

int _cat(char *pathname);
int _pwd(char *pathname);
int _ls(char *pathname);
int _cd(char *pathname);
int _mkdir(char *pathname);
int _rmdir(char *pathname);
int _rm(char *pathname);
int _quit();
int _get(char *pathname);
int _put(char *pathname);


typedef void(*cmd)();
char *command_str[20] = { "lcat, lpwd, lls, lcd, lmkdir, lrmdir, lrm", "quit", "get", "put" };
cmd command[20] = { cat, pwd, ls, cd, mkdir, rmdir, rm, quit, get, put};


int main(int argc, char *argv[])
{
	int cfd;
	struct sockaddr_in saddr;

	printf("1. create a TCP socket\n");
	cfd = socket(AF_INET, SOCK_STREAM, 0);
	if (cfd < 0) {
		printf("socket creation failed\n");
		exit(1);
	}

	printf("2. fill in [localhost IP port=1234] as server address\n");
	bzero(&saddr, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_port = htons(PORT);

	printf("3. connect client socket to server\n");
	if (connect(cfd, (struct sockaddr *)&saddr, sizeof(saddr)) != 0) {
		printf("connection to server failed\n");
		exit(2);
	}

	//processing loop
	while (1) {
		//print available commands

		//get command
		printf("input a line : ");
		bzero(line, MAX);                // zero out line[ ]
		fgets(line, MAX, stdin);         // get a line (end with \n) from stdin

		line[strlen(line) - 1] = 0;        // kill \n at end
		if (line[0] == 0)                  // exit if NULL line
			exit(0);

		// Send ENTIRE line to server
		n = write(cfd, line, MAX);
		printf("client: wrote n=%d bytes; line=(%s)\n", n, line);
		
		
		/*
		// Read a line from sock and show it
		n = read(cfd, ans, MAX);
		printf("client: read  n=%d bytes; echo=(%s)\n", n, ans);
		*/
		//recieve reply
		while (strcmp(ans, "OK")) {
			//n = read(cfd, ans, MAX);
			//printf("client: read  n=%d bytes; echo=(%s)\n", n, ans);
		}

	}

}





int _cat(char *pathname) {

}

int _pwd(char *pathname) {
	line[256];
	getcwd(line, 256);
	printf("%s", line);
}

int _ls(char *pathname) {

}

int _cd(char *pathname) {

}

int _mkdir(char *pathname) {
	return mkdir(pathname, 0755);
}

int _rmdir(char *pathname) {
	return rmdir(pathname);
}

int _rm(char *pathname) {
	return unlink(pathname);
}

int _quit() {
	exit(0);
}

int _get(char *pathname) {

}

int _put(char *pathname) {

}








