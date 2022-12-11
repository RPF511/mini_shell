#include "builtin.h"


//return 0: builtin | -1 : external command
int builtin_handler(const char *command,char *const argv[]){
    if(strcmp(command,"cd")==0){
        cd(command,argv);
    }



    return -1;
}