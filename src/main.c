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

    gethostname(hostname, HOSTNAMESIZE-1);
    while(1){
        if(isrefresh) print_prompt_msg(username,hostname);
        write_on_fd(STDOUT_FILENO,commands[cmd_idx]);
        if(!input_handler(commands, &cmd_idx)){
            isrefresh = 0;
            if(cmd_idx == cmd_front-1) cmd_idx = cmd_front;
            if(cmd_idx == cmd_rear) cmd_idx = cmd_rear-1;
        }else{
            isrefresh = 1;
            write_on_fd(STDOUT_FILENO,"\n");
            strcpy(current_command,commands[cmd_idx]);

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

        }
    }
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
    if(getcwd(cwd_buffer,BUFSIZE) == NULL)return;
    if(strncmp(cwd_buffer,"/home",5)){
        write_on_fd(STDOUT_FILENO,cwd_buffer);
    }else{
        write_on_fd(STDOUT_FILENO,"~");
        write_on_fd(STDOUT_FILENO,cwd_buffer+6+strlen(uname));
    }
    write_on_fd(STDOUT_FILENO,ANSI_COLOR_RESET);
    write_on_fd(STDOUT_FILENO,ANSI_OFF);
    write_on_fd(STDOUT_FILENO,"$ ");
}