
#define ISDEV 1

#define BUFSIZE 2048
#define COMMAND_QUEUE_SIZE 100

typedef struct command_line{
    int cmd_start;      //command start idx
    int cmd_end;        //command end idx
    int cmd_line_size;  //not changed

    int res_idx;

    int next_start;
    int bracket;
    int background;
    int pipe_idx;
    int input_idx;
    int output_idx;
    int error_idx;
    int ex_idx;
    int output_status;


    char cmd_line[BUFSIZE];
    
    char * result[BUFSIZE];
} command_line;

typedef struct process_handler{
    pid_t pid_list[BUFSIZE];
    int state;
    int pid_num;
}process_handler;

int stdout_backup;
int stdin_backup;
int stderr_backup;
char commands[COMMAND_QUEUE_SIZE][BUFSIZE];