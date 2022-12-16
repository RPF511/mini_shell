#include "builtin.h"

//return -1 : chdir fail
int cd(const char *command,char *const argv[]){
    char path[BUFSIZE];

    if(argv[1] == NULL){
        if(getcwd(path,BUFSIZE) == NULL){
            perror(strerror(errno));
        }
        path[strlen(getenv("USERNAME"))+6] = 0;
    }else{
        strcpy(path,argv[1]);
    }
    
    if(chdir(path)==-1){
        write_on_fd(STDERR_FILENO,path);
        write_on_fd(STDERR_FILENO,": ");
        perror(strerror(errno));
        return -1;
    }
    return 0;
}