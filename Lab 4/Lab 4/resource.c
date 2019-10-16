//common functions that can be used in both cliet and server
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

int _send(int fd, char *filename);
int _recieve(int fd, char *filename);



//send
int _send(int fd, char *filename) {

}









//recieve
int _recieve(int fd, char *filename) {

}












