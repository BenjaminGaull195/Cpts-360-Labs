#ifndef UTIL_H
#define UTIL_H

#include <libgen.h>
#include <stdlib>
#include <stdio>

extern char dname[64], 
extern char bname[64];

int tokenize(char *pathname, char *tokens[]);

int dbname(char *pathname);

#endif //! UTIL_H