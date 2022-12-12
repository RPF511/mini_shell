#ifndef __PARSER_H__
#define __PARSER_H__

#include <string.h>

#include "minishell.h"

// #include "command.h"


void parse_token(struct command_line * commandline);
void commandline_space(int index, struct command_line * commandline);


#endif