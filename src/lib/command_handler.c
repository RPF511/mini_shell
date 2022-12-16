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

void command_handler(struct command_line *commandline, struct process_handler *p_handler)
{
    int temp_input = -1;
    int temp_output = -1;
    int temp_err = -1;
    pid_t pid_idx = -1;
    pid_t mypid = -1;
    while (commandline->next_start < commandline->cmd_line_size){
        temp_input = -1;
        temp_output = -1;
        temp_err = -1;
        next_command_set(commandline);
        #ifdef ISDEV
        print_command_line(commandline);
        print_command_status(commandline);
        #endif
        

        if (commandline->pipe_idx == -1){
            if (commandline->error_idx != -1){
                temp_err = open(commandline->cmd_line + commandline->error_idx, O_WRONLY | O_CREAT | O_TRUNC, 777);
                if (temp_err == -1){
                    perror(strerror(errno));
                }
                else{
                    dup2(temp_err, STDERR_FILENO);
                }
            }
            if (commandline->input_idx != -1){
                temp_input = open(commandline->cmd_line + commandline->input_idx, O_RDONLY);
                if (temp_input == -1){
                    perror(strerror(errno));
                }
                else{
                    dup2(temp_input, STDIN_FILENO);
                }
            }
            if (commandline->output_idx != -1){
                // gcc 4.3.2 open arg 2->3
                if (commandline->output_status == 1){
                    temp_output = open(commandline->cmd_line + commandline->output_idx, O_WRONLY | O_CREAT | O_TRUNC, 777);
                }
                if (commandline->output_status == 2){
                    temp_output = open(commandline->cmd_line + commandline->output_idx, O_WRONLY | O_APPEND);
                }
                // if(commandline->output_status == 3)
                if (temp_output == -1){
                    perror(strerror(errno));
                }else{
                    dup2(temp_output, STDOUT_FILENO);
                }
            }
            if (commandline->background != -1){
                pid_idx = get_pid_var(p_handler);
                p_handler->pid_list[pid_idx] = fork();
                if (p_handler->pid_list[pid_idx] < 0){
                    perror(strerror(errno));
                }
                if (p_handler->pid_list[pid_idx] == 0){
                    mypid = pid_idx;

                    commandline->cmd_line_size = commandline->cmd_end + strlen(commandline->cmd_line + commandline->cmd_end);
                    if (commandline->bracket != -1){
                        commandline->next_start = commandline->cmd_start;
                        commandline->bracket = -1;
                        // when (bracket)&
                    }
                }
                else{
                    commandline->cmd_start = commandline->next_start;
                    continue;
                }

            }

            if (commandline->bracket != -1){
                pid_idx = get_pid_var(p_handler);
                int pid = fork();
                p_handler->pid_list[pid_idx] = pid;
                if (pid < 0){
                    perror(strerror(errno));
                }
                if (pid == 0){
                    mypid = pid_idx;
                    commandline->cmd_line_size = commandline->cmd_end + strlen(commandline->cmd_line + commandline->cmd_end);
                    commandline->next_start = commandline->cmd_start;
                }
                else{
                    // printf("mypid in %d\n", mypid);
                    if (waitpid(pid, &(p_handler->state), 0) < 0)
                    {
                        perror(strerror(errno));
                    }
                }
            }
        }
        else{
            pid_idx = get_pid_var(p_handler);
            int pid = fork();
            p_handler->pid_list[pid_idx] = pid;
            if (pid < 0){
                perror(strerror(errno));
            }
            if (pid == 0){
                mypid = pid_idx;
                int fd[2];
                if (pipe(fd) == -1){
                    perror(strerror(errno));
                }

                pid_idx = get_pid_var(p_handler);
                int ppid = fork();
                p_handler->pid_list[pid_idx] = ppid;
                if (ppid < 0){
                    perror(strerror(errno));
                }
                if (ppid == 0){
                    close(fd[1]);
                    dup2(fd[0], STDIN_FILENO);
                    if (commandline->next_start < commandline->cmd_line_size)
                        commandline->cmd_line_size = commandline->next_start;
                    mypid = pid_idx;
                    commandline->result[0] = NULL;
                    commandline->next_start = commandline->pipe_idx;
                }
                else{
                    close(fd[0]);
                    dup2(fd[1], STDOUT_FILENO);
                    commandline->cmd_line_size = commandline->cmd_end + strlen(commandline->cmd_line + commandline->cmd_end);
                    if (commandline->bracket != -1){
                        commandline->next_start = commandline->cmd_start;
                        commandline->result[0] = NULL;
                    }
                }
            }
            else
            {
                if (waitpid(pid, &(p_handler->state), 0) < 0)
                {
                    perror(strerror(errno));
                }
            }
        }

        if (commandline->result[0] == NULL) continue;

        execute_command(commandline, p_handler, &mypid);

        if (temp_output != -1)
            close(temp_output);
        if (temp_err != -1)
            close(temp_err);
        if (temp_input != -1)
            close(temp_input);
    };
    // printf("mypid %d\n",mypid);
    if (mypid != -1)
    {
        del_pid_var(p_handler, mypid);
        exit(EXIT_SUCCESS);
    }
}

void execute_command(struct command_line * commandline, process_handler * p_handler , int * mypid){
    if(builtin_handler(commandline->result[0],commandline->result) < 0){
        int pid_idx = get_pid_var(p_handler);
        int pid = fork();
        p_handler->pid_list[pid_idx] = pid;
        if(pid < 0){
            perror(strerror(errno));
        }
        if(pid == 0){
            *mypid = pid_idx;
            // printf("exec : %s\n",commandline->result[0]);
            execvp(commandline->result[0],commandline->result);
            perror(strerror(errno));
        }
        else{
            //todo : handle WCONTINUED or WNOHANG  -> when child error or waiting
            if(waitpid(pid,&(p_handler->state),0)<0){
                perror(strerror(errno));
            }
        }
    }
}

void backup_std(int stdout_backup,int stdin_backup,int stderr_backup){

    dup2(stdout_backup,STDOUT_FILENO);
    dup2(stdin_backup,STDIN_FILENO);
    dup2(stderr_backup,STDERR_FILENO);
}

void next_command_set(struct command_line * commandline){
    clear_command_handler(commandline);
    
    commandline->cmd_start = commandline->next_start;
    int tokensize=0;
    int i = 0;
    int bopen = 0;
    for(i= commandline->cmd_start ; i< commandline->cmd_line_size;){
        // printf("%s\n",commandline->cmd_line+i);
        tokensize = strlen(commandline->cmd_line+i);
        if(!bopen){
            if(!strcmp(commandline->cmd_line+i, ";")){
                if(commandline->ex_idx != -1) commandline->cmd_end = commandline->ex_idx;
                commandline->next_start = i+2;
                commandline->ex_idx = -1;
                break;
            }
            if(!strcmp(commandline->cmd_line+i, "&")){
                if(commandline->ex_idx != -1) commandline->cmd_end = commandline->ex_idx;
                if(commandline->pipe_idx == -1 && commandline->output_idx == -1 && commandline->input_idx == -1){
                    if(commandline->ex_idx != -1) commandline->cmd_end = commandline->ex_idx;
                    commandline->background = i;
                    commandline->next_start = i+2;
                }
                i+=2;
                continue;
            }
            if(!strcmp(commandline->cmd_line+i, ">")){
                int nextlen = strlen(commandline->cmd_line+i+2);
                if(commandline->output_idx == -1){
                    if(commandline->ex_idx != -1) commandline->cmd_end = commandline->ex_idx;
                    if(nextlen){
                        commandline->output_idx = i+2;
                        commandline->output_status = 1;
                        commandline->next_start = i+nextlen+3;
                        i += nextlen +1 ;
                    }
                }
                i+=2;
                continue;
            }
            if(!strcmp(commandline->cmd_line+i, ">>")){
                int nextlen = strlen(commandline->cmd_line+i+3);
                if(commandline->output_idx == -1){
                    if(commandline->ex_idx != -1) commandline->cmd_end = commandline->ex_idx;
                    if(nextlen){
                        commandline->output_idx = i+3;
                        commandline->output_status = 2;
                        commandline->next_start = i+nextlen+4;
                        i += nextlen +1 ;
                    }
                }
                i+=3;
                continue;
            }
            if(!strcmp(commandline->cmd_line+i, ">|")){
                int nextlen = strlen(commandline->cmd_line+i+3);
                if(commandline->output_idx == -1){
                    if(commandline->ex_idx != -1) commandline->cmd_end = commandline->ex_idx;
                    commandline->next_start = i+3;
                    if(nextlen){
                        commandline->output_idx = i+3;
                        commandline->output_status = 3;
                        commandline->next_start = i+nextlen+4;
                        i += nextlen +1 ;
                    }
                    
                }
                i+=3;
                continue;
            }
            if(!strcmp(commandline->cmd_line+i, "2>")){
                int nextlen = strlen(commandline->cmd_line+i+3);
                if(commandline->error_idx == -1){
                    if(commandline->ex_idx != -1) commandline->cmd_end = commandline->ex_idx;
                    if(nextlen){
                        commandline->error_idx = i+3;
                        commandline->next_start = i+nextlen+4;
                        i += nextlen +1 ;
                    }
                }
                i+=3;
                continue;
            }
            if(!strcmp(commandline->cmd_line+i, "<")){
                int nextlen = strlen(commandline->cmd_line+i+2);
                if(commandline->input_idx == -1){
                    if(commandline->ex_idx != -1) commandline->cmd_end = commandline->ex_idx;
                    if(nextlen){
                        commandline->input_idx = i+2;
                        commandline->next_start = i+nextlen+3;
                        i += nextlen +1 ;
                    }
                }
                i+=2;
                continue;
            }
            if(!strcmp(commandline->cmd_line+i, "|") && commandline->pipe_idx == -1){
                int nextlen = strlen(commandline->cmd_line+i+2);
                if(commandline->ex_idx != -1) commandline->cmd_end = commandline->ex_idx;
                if(nextlen){
                    commandline->pipe_idx = i+2;
                    i += nextlen +1 ;
                }

                i+=2;
                continue;
            }


            if(!strcmp(commandline->cmd_line+i, "(")){
                commandline->bracket = i;
                commandline->cmd_start += tokensize+1;
                i+=2;
                bopen = 1;
                continue;
            }
            if(commandline->bracket != -1) continue;
        }else{
            if(!strcmp(commandline->cmd_line+i, ")")){
                commandline->bracket = i;
                bopen = 0;
                if(commandline->ex_idx != -1) commandline->cmd_end = commandline->ex_idx;
                commandline->next_start = i+2;
                commandline->ex_idx = -1;
                i += 2;
                continue;
            }
        }
       
        if(commandline->bracket == -1 && commandline -> pipe_idx == -1) commandline->result[(commandline->res_idx)++] = commandline->cmd_line+i;
        commandline -> ex_idx = i;
        // commandline->cmd_end = i;
        i += tokensize+1;
    }
    if(commandline->bracket == -1) commandline->result[commandline->res_idx] = NULL;
    if(commandline->next_start <= commandline->cmd_start)commandline->next_start = i;
    if(commandline->cmd_end < commandline->cmd_start)commandline->cmd_end = i;
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
    write_on_fd(STDOUT_FILENO,"\n");
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