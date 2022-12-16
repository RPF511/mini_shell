#include "command.h"


// ')' -> exit(EXIT_SUCCESS)


// typedef struct command_line{
//     int cmd_start;      //current command start idx
//     int cmd_end;        //current command end idx
//     int cmd_line_size;  //index of end : not changed

//     int res_idx;

//     int next_start;
//     int bracket;         //send next start when fork
//     int background;
//     int pipe_idx;
//     int input_idx;
//     int output_idx;
//     int error_idx;
//     int ex_idx;
//     int output_status;    //1 : normal  | 2 : follow | 3 : noclobber

//     char cmd_line[BUFSIZE];
    
//     char * result[BUFSIZE];
// } command_line;

void command_handler(struct command_line * commandline){
    while(commandline->next_start < commandline->cmd_line_size){
        
        next_command_set(commandline);
        print_command_line(commandline);
        print_command_status(commandline);


        builtin_handler(commandline->result[0],commandline->result);

        
        // print_command_line(commandline);
        // print_command_status(commandline);
    };
    

    
        // if(commandline -> bracket != -1){
        // ...
        // }
    
    

}

void next_command_set(struct command_line * commandline){
    clear_command_handler(commandline);
    commandline->cmd_start = commandline->next_start;
    int tokensize=0;
    int i = 0;
    for(i= commandline->cmd_start ; i< commandline->cmd_line_size;){
        tokensize = strlen(commandline->cmd_line+i);
        if(commandline->bracket == -1){
            if(!strcmp(commandline->cmd_line+i, ";")){
                if(commandline->ex_idx != -1) commandline->cmd_end = commandline->ex_idx;
                commandline->next_start = i+2;
                commandline->ex_idx = -1;
                break;
            }
            if(!strcmp(commandline->cmd_line+i, "&")){
                if(commandline->ex_idx != -1) commandline->cmd_end = commandline->ex_idx;
                commandline->background = i;
                commandline->next_start = i+2;
                break;
            }
            if(!strcmp(commandline->cmd_line+i, "|")){
                if(commandline->pipe_idx == -1 && commandline->output_idx == -1 && commandline->input_idx == -1){
                    if(commandline->ex_idx != -1) commandline->cmd_end = commandline->ex_idx;
                    commandline->next_start = i+2;
                    if(strlen(commandline->cmd_line+commandline->next_start)) commandline->pipe_idx = i+2;
                    
                }
                continue;
            }
            if(!strcmp(commandline->cmd_line+i, ">")){
                if(commandline->output_idx == -1){
                    if(commandline->ex_idx != -1) commandline->cmd_end = commandline->ex_idx;
                    commandline->next_start = i+2;
                    if(strlen(commandline->cmd_line+commandline->next_start)){
                        commandline->output_idx = commandline->next_start;
                        commandline->output_status = 1;
                    }
                    
                }
                continue;
            }
            if(!strcmp(commandline->cmd_line+i, ">>")){
                if(commandline->output_idx == -1){
                    if(commandline->ex_idx != -1) commandline->cmd_end = commandline->ex_idx;
                    commandline->next_start = i+3;
                    if(strlen(commandline->cmd_line+commandline->next_start)){
                        commandline->output_idx = commandline->next_start;
                        commandline->output_status = 2;
                    }
                    
                }
                continue;
            }
            if(!strcmp(commandline->cmd_line+i, ">|")){
                if(commandline->output_idx == -1){
                    if(commandline->ex_idx != -1) commandline->cmd_end = commandline->ex_idx;
                    commandline->next_start = i+3;
                    if(strlen(commandline->cmd_line+commandline->next_start)){
                        commandline->output_idx = commandline->next_start;
                        commandline->output_status = 3;
                    }
                    
                }
                continue;
            }
            if(!strcmp(commandline->cmd_line+i, "2>")){
                if(commandline->error_idx == -1){
                    if(commandline->ex_idx != -1) commandline->cmd_end = commandline->ex_idx;
                    commandline->next_start = i+3;
                    if(strlen(commandline->cmd_line+commandline->next_start)){
                        commandline->error_idx = commandline->next_start;
                    }
                    
                }
                continue;
            }
            if(!strcmp(commandline->cmd_line+i, "<")){
                if(commandline->input_idx == -1){
                    if(commandline->ex_idx != -1) commandline->cmd_end = commandline->ex_idx;
                    commandline->next_start = i+2;
                    if(strlen(commandline->cmd_line+commandline->next_start)){
                        commandline->input_idx = commandline->next_start;
                    }
                    
                }
                continue;
            }
            


            if(!strcmp(commandline->cmd_line+i, "(")){
                commandline->bracket = i;
                commandline->cmd_start += tokensize+1;
                continue;
            }
        }else{
            if(!strcmp(commandline->cmd_line+i, ")")){
                commandline->bracket = i;
                if(commandline->ex_idx != -1) commandline->cmd_end = commandline->ex_idx;
                commandline->next_start = i+2;
                commandline->ex_idx = -1;
                break;
            }
        }
       
        
        if(commandline->bracket == -1) commandline->result[(commandline->res_idx)++] = commandline->cmd_line+i;
        commandline->ex_idx = i;
        commandline->cmd_end = i;
        i+=tokensize+1;
    }
    if(commandline->bracket == -1) commandline->result[commandline->res_idx] = NULL;
    if(commandline->cmd_start == commandline->next_start)commandline->next_start = i;
}

void clear_command_handler(struct command_line * commandline){
    commandline->ex_idx = -1;
    commandline->bracket = -1;
    commandline->background = -1;
    commandline->pipe_idx = -1;
    commandline->input_idx = -1;
    commandline->output_idx = -1;
    commandline->error_idx = -1;
    commandline->res_idx = 0;
    commandline->output_status = -1;
}


void init_command_line(command_line * commandline, const char * str){
    commandline->cmd_line_size = strlen(str);
    commandline->cmd_start=0;
    commandline->cmd_end=0;
    strcpy(commandline->cmd_line,str);
    commandline->res_idx=0;
    commandline->result[0] = (char *)0;
    commandline->next_start = 0;
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

void print_command_status(struct command_line *commandline)
{
    printf("size %d %d->%d ", commandline->cmd_line_size, commandline->cmd_start, commandline->cmd_end);
    printf("next %d ", commandline->next_start);
    printf("brac %d ", commandline->bracket);
    printf("bg %d ", commandline->background);
    printf("pipe %d ", commandline->pipe_idx);
    printf("input %d ", commandline->input_idx);
    printf("outpot %d ", commandline->output_idx);
    printf("err %d " ,commandline->error_idx);
    printf("out_stat %d ", commandline->output_status);
    printf("\n");
}