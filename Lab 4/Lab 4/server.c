#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>

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

typedef void(*cmd)(int, char *);

int find_cmd(char *cmd);

int _pwd(int fd, char *pathname);
int _ls(int fd, char *pathname);
int _cd(int fd, char *pathname);
int _mkdir(int fd, char *pathname);
int _rmdir(int fd, char *pathname);
int _rm(int fd, char *pathname);
int _get(int fd, char *pathname); //send
int _put(int fd, char *pathname); //recieve

char *command_str[20] = { "pwd", "ls", "cd", "mkdir", "rmdir", "rm", "get", "put", 0};
cmd command[20] = { _pwd, _ls, _cd, _mkdir, _rmdir, _rm, _get, _put };



struct stat mystat, *sp;
char *t1 = "xwrxwrxwr-------";
char *t2 = "----------------";



int main(int argc, char *argv[])
{
	int sfd, cfd, len, i;
	struct sockaddr_in saddr, caddr;
	int in1, in2, out;
	char *_command, *temp;



	// wait for client
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
		//connect to client
		printf("server accepting connection\n");
		cfd = accept(sfd, (struct sockaddr *)&caddr, &len);
		if (cfd < 0) {
			printf("server acccept failed\n");
			exit(4);
		}
		printf("server acccepted a client connection\n");

		//processing loop
		while (1) {
			//read command input
			printf("server: ready for next request\n");
			n = read(cfd, line, MAX);
			if (n == 0) {
				printf("server: client died, server loops\n");
				close(cfd);
				break;
			}
			_command[0] = temp[0] = 0;
			//parse command
			sscanf(line, "%s %s", _command, temp);

			//execute command
			i = find_cmd(_command);
			if (i >= 0) {
				command[i](cfd, temp);
			}
			else {
				printf("server: can't find command: %s\n", _command);
				write(cfd, "Error: can't find command", MAX);
			}

		}
	}
}

//return index for command
int find_cmd(char *cmd) {
	int i = 0;
	for (i = 0; i < 20, command_str[i] != 0; ++i) {
		if (!strcmp(command_str[i], cmd)) {
			return i;
		}
	}
	return -1;
}

//send pwd to client (server)
int _pwd(int fd, char *pathname) {
	char line[MAX];
	getcwd(line, MAX);
	n = write(fd, line, MAX);
	write(fd, "OK pwd\n", MAX);
	printf("server: wrote n=%d bytes; ECHO=[%s]\n", n, line);
	printf("server: wrote n=%d bytes; ECHO=[%s]\n", n, "OK");

}

//ls helper function (server)
int ls_file(int fd, char *fname) {
	struct stat fstat, *sp;
	int r, i;
	char ftime[64], linkname[128], line[MAX], temp[128];
	sp = &fstat;
	if ((r = lstat(fname, &fstat)) < 0) {
		printf("server: can't stat %s<p>", fname);
		//exit(1);
		return -1;
	}// get file mode
	if ((sp->st_mode & 0xF000) == 0x8000) {
		strcat(line, '-');
		//printf("%c", '-');
	}
	if ((sp->st_mode & 0xF000) == 0x4000) {
		strcat(line, 'd');
		//printf("%c", 'd');
	}
	if ((sp->st_mode & 0xF000) == 0xA000) {
		//printf("%c", 'l');
		strcat(line, 'l');
	}
	for (i = 8; i >= 0; --i) {// get file permissions
		if (sp->st_mode & (1 << i)) {
			//printf("%c", t1[i]);
			strcat(line, t1[i]);
		}
		else {
			strcat(line, t2[i]);
			//printf("%c", t2[i]);
		}
	}

	//format file stat
	sprintf(line, " %s %4d %4d %4d %8d", line,  sp->st_nlink, sp->st_gid, sp->st_uid, sp->st_size);
	//printf("%4d", sp->st_nlink);
	//printf("%4d", sp->st_gid);
	//printf("%4d", sp->st_uid);
	//printf("%8d", sp->st_size);
	strcpy(ftime, ctime(&sp->st_ctime));	//format file time
	ftime[strlen(ftime) - 1] = 0;
	sprintf(line, "%s %s", ftime, basename(fname));
	//printf("%s", ftime);
	//printf("%s", basename(fname)); get file symbolic link
	if ((sp->st_mode & 0xF000) == 0xA000) {
		readlink(fname, linkname, 128);
		//printf(" - > %s", linkname);
		sprintf(line, " -> %s", linkname);
	}
	//send file info
	write(fd, line, MAX);
}

//ls helper function 
int ls_dir(int fd, char *dname) {
	struct dirent *ep;
	DIR *dp = opendir(dname);

	if (dp == 0) {
		printf("opendir %s failed\n", dname);
		return -1;
	}
	while (ep = readdir(dp)) {
		if (strcmp(ep->d_name, ".") && strcmp(ep->d_name, "..")) {
			if (ls_file(fd, ep->d_name) < 0) {//send dir info
				return -1;
			}
		}
	}

}

//send directory content info (server)
int _ls(int fd, char *pathname) {
	struct stat mystat, *sp = &mystat;
	int r;
	if (r = lstat(pathname, sp) < 0) {
		printf("server:%s does not exist<p>", pathname);
		write(fd, "server: does not exist\n", MAX);
		return -1;
	}
	if (S_ISDIR(sp->st_mode)) {//if dir
		if (ls_dir(fd, pathname) < 0) {
			write(fd, "server: ls failed\n", MAX);
			printf("server: ls failed\n");
			return -1;
		}
	}
	else { //if file
		if (ls_file(fd, pathname) < 0) {
			write(fd, "server: ls failed\n", MAX);
			printf("server: ls failed\n");
			return -1;
		}
		write(fd, "END of ls\n", MAX);
	}
}

//change cwd (server)
int _cd(int fd, char *pathname) {
	int n;
	int r = chdir(pathname);
	n = write(fd, "OK cd\n", MAX);
	printf("server: wrote n=%d bytes; ECHO=[%s]\n", n, "OK");
}

//make new dir (server)
int _mkdir(int fd, char *pathname) {
	int n;
	int r = mkdir(pathname, 0755);
	n = write(fd, "OK mkdir\n", MAX);
	printf("server: wrote n=%d bytes; ECHO=[%s]\n", n, "OK");
}

//remove existing dir *server)
int _rmdir(int fd, char *pathname) {
	int n;
	int r = rmdir(pathname);
	n = write(fd, "OK rmdir\n", MAX);
	printf("server: wrote n=%d bytes; ECHO=[%s]\n", n, "OK");
}

//remove existing file (server)
int _rm(int fd, char *pathname) {
	int n;
	int r = unlink(pathname);
	n = write(fd, "OK rm\n", MAX);
	printf("server: wrote n=%d bytes; ECHO=[%s]\n", n, "OK");
}

//send file to client
int _get(int fd, char *pathname) { //send
	_send(fd, pathname);
}

//get file from client
int _put(int fd, char *pathname) { //recieve
	_recieve(fd, pathname);
}

