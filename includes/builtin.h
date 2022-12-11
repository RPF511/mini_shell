#ifndef __BUILTIN_H__
#define __BUILTIN_H__

#include <string.h>
#include <errno.h>
#include "minishell.h"


int builtin_handler(const char *command,char *const argv[]);
int cd(const char *command,char *const argv[]);

#endif