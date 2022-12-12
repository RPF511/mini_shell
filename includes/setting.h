
#define ISDEV 1

#define BUFSIZE 2048
#define COMMAND_QUEUE_SIZE 100

typedef struct command_line{
    int cmd_start;
    int cmd_end;
    int cmd_line_size;

    int res_idx;

    char cmd_line[BUFSIZE];
    
    char * result[BUFSIZE];
} command_line;