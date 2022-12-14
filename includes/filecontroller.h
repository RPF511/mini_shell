#ifndef __FILECONTROLLER_H__
#define __FILECONTROLLER_H__

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <termios.h>

#include "minishell.h"



#define ASCII_ESC 27
#define ASCII_SQ_BRACKET_OP 91
#define ASCII_SQ_BRACKET_CL 93
#define ASCII_NORM_BRACKET_OP 40
#define ASCII_NORM_BRACKET_CL 41
#define ASCII_UP_A 65
#define ASCII_RIGHT_C 67
#define ASCII_DOWN_B 66
#define ASCII_LEFT_D 68
#define ASCII_BACKSPACE 127
#define ASCII_PERCENT 37
#define ASCII_AND 38
#define ASCII_SEMICOLON 59
#define ASCII_GREATER 62
#define ASCII_LESSER 60
#define ASCII_CTRL_D 4
#define ASCII_CTRL_C 3
#define ASCII_ENTER 10


ssize_t write_on_fd(int fd, char * data);
int input_handler(char commands[COMMAND_QUEUE_SIZE][BUFSIZE], int * cmd_idx, int cmd_rear);
int getch(void);
int getche(void);

#endif