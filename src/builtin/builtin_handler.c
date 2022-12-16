#include "builtin.h"


//return 0: builtin | -1 : external command
int builtin_handler(const char *command,char *const argv[]){
    if(command == NULL){
        // perror(strerror(errno));
        return -1;
    }
    if(strcmp(command,"cd")==0){
        cd(command,argv);
    }

    printf("here\n");

    return -1;
}