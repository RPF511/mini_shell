#include "builtin.h"

//return -1 : chdir fail
int history(const char *command,char *const argv[]){
    char temp[BUFSIZE];
    for(int i =0; i<COMMAND_QUEUE_SIZE; i++){
        if(!strlen(commands[i]))break;
        sprintf(temp,"%d ",i);
        // strcat(temp," ");
        strcat(temp,commands[i]);

        write_on_fd(STDOUT_FILENO,temp);
        write_on_fd(STDOUT_FILENO,"\n");
    }
    write_on_fd(STDOUT_FILENO,"\n");
    return 0;
}