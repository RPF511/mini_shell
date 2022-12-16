
#ifndef __minishell_h__
#define __minishell_h__

#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#include "setting.h"
#include "filecontroller.h"
#include "builtin.h"
#include "parser.h"
#include "command.h"
#include "processhandler.h"

#define ANSI_COLOR_GREEN    "\x1b[32m"
#define ANSI_COLOR_BLUE     "\x1b[34m"
#define ANSI_COLOR_RESET    "\x1b[0m"
#define ANSI_BOLD           "\e[1m"
#define ANSI_OFF            "\e[m"


#define COMMAND_QUEUE_SIZE 100
#define HOSTNAMESIZE 10




void print_stat(void);
void print_prompt_msg(char * uname, char *hname);
void shell_mainloop(void);



#endif