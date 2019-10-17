#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>

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

int find_cmd(char *cmd);

int _cat(int fd, char *pathname);
int _pwd(int fd, char *pathname);
int _ls(int fd, char *pathname);
int _cd(int fd, char *pathname);
int _mkdir(int fd, char *pathname);
int _rmdir(int fd, char *pathname);
int _rm(int fd, char *pathname);
int _quit(int fd, char *pathname);
int _get(int fd, char *pathname);
int _put(int fd, char *pathname);


typedef void(*cmd)(int, char *);
char *command_str[20] = { "lcat, lpwd, lls, lcd, lmkdir, lrmdir, lrm", "quit", "get", "put", 0 };
cmd command[20] = { _cat, _pwd, _ls, _cd, _mkdir, _rmdir, _rm, _quit, _get, _put};

struct stat mystat, *sp;
char *t1 = "xwrxwrxwr-------";
char *t2 = "----------------";


int main(int argc, char *argv[])
{
	int cfd, i, m;
	char *_cmd, *temp, blk[BLKSIZE];
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

		_cmd[0] = temp[0] = 0;

		if (line[0] != 'l') {	//server command
			// Send ENTIRE line to server
			sscanf(line, "%s %s", _cmd, temp);
			n = write(cfd, line, MAX);
			printf("client: wrote n=%d bytes; line=(%s)\n", n, line);
			if (!strcmp(_cmd, "get")) {
				_get(cfd, temp);
			}
			else if (!strcmp(_cmd, "put")) {
				_put(cfd, temp);
			}
			else {

				while (read(cfd, line, MAX)) {
					
				}
			}
		}
		else {	//local commands, get, put 
			sscanf(line, "%s %s", _cmd, temp);
			i = find_cmd(_cmd);
			command[i](cfd, temp);
			
		}
		
		/*
		// Read a line from sock and show it
		n = read(cfd, ans, MAX);
		printf("client: read  n=%d bytes; echo=(%s)\n", n, ans);
		*/
		//recieve reply

		

	}

}


int find_cmd(char *cmd) {
	int i = 0;
	while (command_str[i]) {
		if (strcmp(cmd, command[i]) == 0) {
			return i;
		}
		++i;
	}
	return -1;
}



int _cat(int fd, char *pathname) {
	int gd, i, m, n;
	char buf[BLKSIZE], dummy;
	gd = open(pathname, O_RDONLY);
	if (gd < 0) {
		printf("failed to open %s\n", pathname);
		return -1;
	}
	while (n = read(gd, buf, BLKSIZE)) {
		m = write(1, buf, n);
		//printf("<p>");
	}
}

int _pwd(int fd, char *pathname) {
	line[256];
	getcwd(line, 256);
	printf("%s\n", line);
}

int ls_file(char *fname) {
	struct stat fstat, *sp;
	int r, i;
	char ftime[64], linkname[128];
	sp = &fstat;
	if ((r = lstat(fname, &fstat)) < 0) {
		printf("can't stat %s\n", fname);
		//exit(1);
		return -1;
	}
	if ((sp->st_mode & 0xF000) == 0x8000) {
		printf("%c", '-');
	}
	if ((sp->st_mode & 0xF000) == 0x4000) {
		printf("%c", 'd');
	}
	if ((sp->st_mode & 0xF000) == 0xA000) {
		printf("%c", 'l');
	}
	for (i = 8; i >= 0; --i) {
		if (sp->st_mode & (1 << i)) {
			printf("%c", t1[i]);
		}
		else {
			printf("%c", t2[i]);
		}
	}
	printf("%4d", sp->st_nlink);
	printf("%4d", sp->st_gid);
	printf("%4d", sp->st_uid);
	printf("%8d", sp->st_size);
	strcpy(ftime, ctime(&sp->st_ctime));
	ftime[strlen(ftime) - 1] = 0;
	printf("%s", ftime);
	printf("%s", basename(fname));
	if ((sp->st_mode & 0xF000) == 0xA000) {
		readlink(fname, linkname, 128);
		printf(" - > %s", linkname);
	}
	printf("\n");
}

int ls_dir(char *dname) {
	struct dirent *ep;
	DIR *dp = opendir(dname);

	if (dp == 0) {
		printf("opendir %s failed\n", dname);
		return -1;
	}
	while (ep = readdir(dp)) {
		if (strcmp(ep->d_name, ".") && strcmp(ep->d_name, "..")) {
			ls_file(ep->d_name);
		}
	}

}

int _ls(int fd, char *pathname) {
	struct stat mystat, *sp = &mystat;
	int r;
	if (r = lstat(pathname, sp) < 0) {
		printf("%s does not exist\n", pathname);
		return -1;
	}
	if (S_ISDIR(sp->st_mode)) {
		ls_dir(pathname);
	}
	else {
		ls_file(pathname);
	}
}

int _cd(int fd, char *pathname) {
	int r = chdir(pathname);
	return 0;
}

int _mkdir(int fd, char *pathname) {
	return mkdir(pathname, 0755);
	return 0;
}

int _rmdir(int fd, char *pathname) {
	return rmdir(pathname);
	return 0;
}

int _rm(int fd, char *pathname) {
	return unlink(pathname);
	return 0;
}

int _quit(int fd, char *pathname) {
	exit(0);
}

int _get(int fd, char *pathname) {
	_recieve(fd, pathname);
}

int _put(int fd, char *pathname) {
	_send(fd, pathname);
}








