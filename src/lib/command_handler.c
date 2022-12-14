#include "command.h"


// ')' -> exit(EXIT_SUCCESS)


// typedef struct command_line{
//     int cmd_start;
//     int cmd_end;
//     int cmd_line_size;

//     int res_idx;

//     int next_start;
//     int bracket;
//     int background;
//     int pipe_redir;
//     int cmd_set_end;


//     char cmd_line[BUFSIZE];
    
//     char * result[BUFSIZE];
// } command_line;

void command_handler(struct command_line * commandline){
    // while()
    next_command_set(commandline);

    
        // if(commandline -> bracket != -1){
        // ...
        // }
    
    builtin_handler(commandline->result[0],commandline->result);

}

void next_command_set(struct command_line * commandline){
    clear_command_handler(commandline);
    int tokensize=0;
    for(int i= commandline->cmd_start ; i< commandline->cmd_line_size;){
        tokensize = strlen(commandline->cmd_line+i);






        commandline->result[(commandline->res_idx)++] = commandline->cmd_line+i;
        
        i+=tokensize+1;
    }
    // next_start =
    commandline->result[commandline->res_idx] = NULL;
    print_command_line(commandline);
}

void clear_command_handler(struct command_line * commandline){
    commandline->next_start = -1;
    commandline->bracket = -1;
    commandline->background = -1;
    commandline->pipe_redir = -1;
    commandline->cmd_set_end = -1;
    commandline->res_idx = 0;
}


void init_command_line(command_line * commandline, const char * str){
    commandline->cmd_line_size = strlen(str);
    commandline->cmd_start=0;
    commandline->cmd_end=0;
    strcpy(commandline->cmd_line,str);
    commandline->res_idx=0;
    commandline->result[0] = (char *)0;
    clear_command_handler(commandline);
    // #ifdef ISDEV
    // printf("command init done : %s |size : %d\n",commandline->cmd_line,commandline->cmd_line_size);
    // #endif
}

void print_command_line(command_line * commandline){
    char tempstr[BUFSIZE];
    write_on_fd(STDOUT_FILENO,"cmd_line : [");
    for(int i= 0; i< commandline->cmd_line_size;){
        strcpy(tempstr,commandline->cmd_line+i);
        write_on_fd(STDOUT_FILENO,tempstr);
        write_on_fd(STDOUT_FILENO,", ");
        i+=strlen(tempstr)+1;
    }
    write_on_fd(STDOUT_FILENO,"]\nresult : [");
    for(int i= 0; i< commandline->res_idx;i++){
        write_on_fd(STDOUT_FILENO,commandline->result[i]);
        write_on_fd(STDOUT_FILENO,", ");
    }
    write_on_fd(STDOUT_FILENO,"]\n");
}