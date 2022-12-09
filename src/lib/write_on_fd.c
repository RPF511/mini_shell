#include "filecontroller.h"

ssize_t write_on_fd(int fd, char * data){
    if(data == NULL)return -1;
    return write(fd,data,strlen(data));
}