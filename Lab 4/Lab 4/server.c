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

char line[MAX];
int n;

typedef void(*cmd)(char *);

int _pwd(char *pathname);
int _ls(char *pathname);
int _cd(char *pathname);
int _mkdir(char *pathname);
int _rmdir(char *pathname);
int _rm(char *pathname);
int _get(char *pathname); //send
int _put(char *pathname); //recieve

char *command_str[20] = ("pwd", "ls", "cd", "mkdir", "rmdir", "rm", "get", "put");
cmd command[20] = { _pwd, _ls, _cd, _mkdir, _rmdir, _rm, _get, _push };

int main(int argc, char *argv[])
{
	int sfd, cfd, len;
	struct sockaddr_in saddr, caddr;
	int in1, in2, out;


	printf("1. create a TCP socket\n");
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sfd < 0) {
		printf("socket creation failed\n");
		exit(1);
	}

	printf("2. fill in [localhost IP port=1234] as server address\n");
	bzero(&saddr, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons(PORT);

	printf("3. bind socket with server address\n");
	if ((bind(sfd, (struct sockaddr *)&saddr, sizeof(saddr))) != 0) {
		printf("socket bind failed\n");
		exit(2);
	}

	printf("4. server listens\n");
	if ((listen(sfd, 5)) != 0) {
		printf("Listen failed\n");
		exit(3);
	}

	len = sizeof(caddr);
	while (1) {
		printf("server accepting connection\n");
		cfd = accept(sfd, (struct sockaddr *)&caddr, &len);
		if (cfd < 0) {
			printf("server acccept failed\n");
			exit(4);
		}
		printf("server acccepted a client connection\n");

		//processing loop
		while (1) {

		}
	}
}


int _pwd(char *pathname) {

}

int _ls(char *pathname) {

}

int _cd(char *pathname) {

}

int _mkdir(char *pathname) {

}

int _rmdir(char *pathname) {

}

int _rm(char *pathname) {

}

int _get(char *pathname) { //send

}

int _put(char *pathname) { //recieve

}

