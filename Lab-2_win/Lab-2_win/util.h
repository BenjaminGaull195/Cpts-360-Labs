#ifndef UTIL_H
#define UTIL_H

#include "command.h"

#include "libgen.h"
#include <stdlib.h>
#include <stdio.h>

extern char dname[64], bname[64];



//int tokenize(char *pathname, char *tokens[]);

int dbname(char *pathname);

#endif //! UTIL_H