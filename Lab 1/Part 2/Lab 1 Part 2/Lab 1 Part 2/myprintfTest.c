#include "stdio.h"
#include "myprintf.h"

int main(int argc, char *argv[], char* env[]) {
	char a = 'a';
	char b[20] = "hello world";
	int c= -40;
	unsigned int d = 45;

	//printf("a = %c; b = %s; c = %d; d = %u; e = %o; f = %x\n", a, b, c, d, d, d);
	myprintf("a = %c; b = %s; c = %d; d = %u; e = %o; f = %x", &a, &b, &c, &d, &d, &d);




	return 0;
}