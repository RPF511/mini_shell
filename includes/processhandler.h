#ifndef __PROCESSHANDLER_H__
#define __PROCESSHANDLER_H__

#include "minishell.h"


void init_p_handler(process_handler * p_handler);
int get_pid_var(process_handler * p_handler);
void del_pid_var(process_handler * p_handler, int index);
void get_pid_list(process_handler * p_handler, int *buffer);

#endif