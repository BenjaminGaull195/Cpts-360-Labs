#include "util.h"


//int tokenize(char *pathname, char *tokens[]) {
//	int count = 0;
//	tokens[count] = strtok(path, "/");  // first call to strtok()
//	while(tokens[count]){   
//		++count;
//		printf(“%s “, s);
//		tokens[count] = strtok(0, "/");  // call strtok() until it returns NULL
//	}
//}

int dbname(char *pathname) {
	char temp[128];  // dirname(), basename() destroy original pathname
	strcpy(temp, pathname);
	strcpy(dname, dirname(temp));
	strcpy(temp, pathname);
	strcpy(bname, basename(temp));
}