#include "processhandler.h"


void init_p_handler(process_handler * p_handler){
    for(int i = 0; i <BUFSIZE ;i++){
        p_handler -> pid_list[i] = -1;
    }
    p_handler -> state = 0;
    p_handler -> pid_num = 0;
}

int get_pid_var(process_handler * p_handler){
    int res = -1;
    for(int i = 0; i <BUFSIZE ;i++){
        if(p_handler -> pid_list[i] == -1){
            res = i;
            break;
        }
    }
    return res;
}

void del_pid_var(process_handler * p_handler, int index){
    if(p_handler -> pid_list[index] == -1){
        perror("pid not allocated");
    }else{
        p_handler -> pid_list[index] = -1;
    }
}

void get_pid_list(process_handler * p_handler, int *buffer){
    int index = 0;
    for(int i = 0; i <BUFSIZE ;i++){
        if(p_handler -> pid_list[i] == -1){
            buffer[index++] = i;
        }
    }
}
