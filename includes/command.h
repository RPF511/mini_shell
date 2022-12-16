#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "minishell.h"

void command_handler(struct command_line * commandline);
void next_command_set(struct command_line * commandline);
void init_command_line(struct command_line * commandline, const char * str);
void print_command_line(struct command_line * commandline);
void clear_command_handler(struct command_line * commandline);
void print_command_status(struct command_line * commandline);
void backup_std(int stdout_backup,int stdin_backup,int stderr_backup);


#endif