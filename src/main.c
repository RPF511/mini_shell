#include "minishell.h"

char cwd_buffer[BUFSIZE];




int main(int argc, char * argv[], char *envp[]){
    print_stat();

    shell_mainloop();

    return 0;
}

void shell_mainloop(){
    char * username = getenv("USERNAME");
    char hostname[HOSTNAMESIZE];
    char commands[COMMAND_QUEUE_SIZE][BUFSIZE];
    char current_command[BUFSIZE];
    int cmd_idx = 0;
    int cmd_front = 0;
    int cmd_rear = 1;
    memset(commands,0,COMMAND_QUEUE_SIZE*BUFSIZE);
    int isrefresh = 1;
    command_line * commandline = (command_line *)malloc(sizeof(command_line));

    gethostname(hostname, HOSTNAMESIZE-1);
    while(1){
        if(isrefresh) print_prompt_msg(username,hostname);
        write_on_fd(STDOUT_FILENO,commands[cmd_idx]);
        if(input_handler(commands, &cmd_idx, cmd_rear) == -1){
            isrefresh = 0;
            if(cmd_idx == cmd_front-1) cmd_idx = cmd_front;
            if(cmd_idx == cmd_rear) cmd_idx = cmd_rear-1;
        }else{
            isrefresh = 1;
            write_on_fd(STDOUT_FILENO,"\n");
            // printf("c : %s\n",commands[cmd_idx]);
            if(strlen(commands[cmd_idx])){
                strcpy(current_command,commands[cmd_idx]);

                //command queue adjust
                if(cmd_idx != cmd_rear-1){
                    strcpy(commands[cmd_rear-1],current_command);
                    cmd_idx = cmd_rear;
                }else{
                    cmd_idx++;
                }
                if(++cmd_rear == COMMAND_QUEUE_SIZE) cmd_rear = 0;
                if(cmd_rear == cmd_front) cmd_front++;
                // #ifdef ISDEV
                // printf("command : %s / fr : %d / re :%d / cur : %d\n",current_command,cmd_front,cmd_rear,cmd_idx);
                // #endif

                init_command_line(commandline, current_command);
                parse_token(commandline);
                // print_command_line(commandline);
                // next_command_set(commandline);
                command_handler(commandline);
            }
        }
        
    }

    free(commandline);
}



void print_stat(){
    #ifdef ISDEV
        printf("devmode on\n");
    #endif
}

void print_prompt_msg(char * uname, char *hname){
    write_on_fd(STDOUT_FILENO,ANSI_BOLD);
    write_on_fd(STDOUT_FILENO,ANSI_COLOR_GREEN);
    write_on_fd(STDOUT_FILENO,uname);
    write_on_fd(STDOUT_FILENO,"@");
    write_on_fd(STDOUT_FILENO,hname);
    write_on_fd(STDOUT_FILENO,ANSI_COLOR_RESET);
    write_on_fd(STDOUT_FILENO,":");
    write_on_fd(STDOUT_FILENO,ANSI_BOLD);
    write_on_fd(STDOUT_FILENO,ANSI_COLOR_BLUE);
    // handle error and exit
    if(getcwd(cwd_buffer,BUFSIZE) == NULL){
        perror(strerror(errno));
    }
    int isuserdir = 0;
    if(!strncmp(cwd_buffer,"/home",5)){
        if(cwd_buffer[5]!=0 && !strncmp(cwd_buffer+6,uname,strlen(uname))){
            isuserdir = 1;
            cwd_buffer[strlen(uname)+5] = '~';
            write_on_fd(STDOUT_FILENO,cwd_buffer+5+strlen(uname));
        }
    }
    if(!isuserdir) write_on_fd(STDOUT_FILENO,cwd_buffer);
    write_on_fd(STDOUT_FILENO,ANSI_COLOR_RESET);
    write_on_fd(STDOUT_FILENO,ANSI_OFF);
    write_on_fd(STDOUT_FILENO,"$ ");
}


