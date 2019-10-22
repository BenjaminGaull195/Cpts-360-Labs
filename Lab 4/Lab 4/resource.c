//common functions that can be used in both cliet and server
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

int _send(int fd, char *filename);
int _recieve(int fd, char *filename);



//send file
int _send(int fd, char *filename) {
	int gd, size, n, total, r;
	struct stat mystat, *sp;
	char buf[BLKSIZE], ans[MAX];
	printf("sending %s\n", filename);

	//get file stat
	printf("1. stat %s ", filename);
	sp = &mystat;
	r = stat(filename, sp);
	if (r < 0) {
		printf("can't stat %s\n", filename);
		sprintf(ans, "BAD can't stat %s\n", filename);
		write(fd, ans, MAX);
		return -1;
	}

	if (!S_ISREG(sp->st_mode)) {
		printf("%s is not REG file\n", filename);
		sprintf(ans, "BAD %s is not REG file\n", filename);
		write(fd, ans, MAX);
		return -1;
	}

	gd = open(filename, O_RDONLY);
	if (gd < 0) {
		printf("can't open %s for READ\n", filename);
		sprintf(ans, "BAD can't open %s for READ\n", filename);
		write(fd, ans, MAX);
		return -1;
	}
	printf("2. send OK %d\n", sp->st_size);
	sprintf(ans, "OK %d", sp->st_size);
	write(fd, ans, MAX);

	total = 0;

	//open file for read/send
	printf("3. open %s for READ\n", filename);
	while (n = read(gd, buf, BLKSIZE)) {	//loop and read file
		write(fd, buf, n);	//send file
		total += n;
		printf("n = %d, total = %d\n", n, total);
		bzero(buf, BLKSIZE);
	}
	printf("sent %d bytes\n", total);
	return total;
}

//recieve
int _recieve(int fd, char *filename) {
	int gd, size, n, total, r;
	char buf[BLKSIZE], ans[MAX], *responce, *num;
	printf("recieving %s", filename);

	//open file for write/create
	printf("1. open % for WRITE\n", filename);
	gd = open(filename, O_WRONLY | O_CREAT);
	if (gd < 0) {
		printf("failed to open %s for WRITE\n", filename);
		return -1;
	}
	
	printf("2. Waiting for responce\n");
	read(fd, ans, MAX);
	sscanf(ans, "%s %s", responce, num);

	//if server ready to send
	if (strcmp(responce, "OK")) {
		r = atoi(num);

		printf("3. WRITE %d bytes to %s\n", r, filename);
		while (n = read(fd, buf, BLKSIZE)) {	//read file from server
			write(gd, buf, n);	//write to file
			total += n;
			printf("n = %d, total = %d\n", n, total);
			bzero(buf, BLKSIZE);
		}
	}
	else {
		printf("Error: can't get file\n");
	}
	return total;
}

