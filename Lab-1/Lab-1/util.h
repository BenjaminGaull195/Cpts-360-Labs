#ifndef UTIL_H
#define UTIL_H

#include "command.h"

#include <libgen.h>
#include <stdlib>
#include <stdio>

 


int tokenize(char *pathname, char *tokens[]);

int dbname(char *pathname);

#endif //! UTIL_H