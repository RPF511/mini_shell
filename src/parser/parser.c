#include "parser.h"

void parse_token(struct command_line * commandline){
    int isspace = 1;   // was there space before
    int isdup2 = 0;
    for(int i = 0;i < commandline -> cmd_line_size ; i++){
        char character = commandline->cmd_line[i];
        // #ifdef ISDEV
        // printf("[%c] i %d size %d isspace %d isdup2 %d \n",character,i,commandline -> cmd_line_size,isspace, isdup2);
        // for(int j = 0; j < commandline -> cmd_line_size; j++){
        //     if(commandline->cmd_line[j] == 0){
        //         printf(" ");
        //     }else{
        //         printf("%c",commandline->cmd_line[j]);
        //     }
        // }
        // printf("\n\n");
        // #endif

        if(character == ' '){
            // #ifdef ISDEV
            // printf("space\n");
            // #endif
            isdup2 = 0;
            if(isspace){
                for(int j = i ; j < commandline -> cmd_line_size -1 ; j++){
                    commandline->cmd_line[j] = commandline->cmd_line[j+1];
                }
                commandline->cmd_line[--(commandline->cmd_line_size)] = 0;
                i--;
            }
            else{
                isspace = 1;
                commandline->cmd_line[i] = 0;
            }
            continue;
        }
        if(character == '>' || character == '<'){
            // #ifdef ISDEV
            // printf("case b\n");
            // #endif
            if(!isdup2){
                isdup2=1;
                if(!isspace){
                    commandline_space(i,commandline);
                    i++;
                }
                isspace = 0;
            }
            else{
                isdup2 = 0;
                commandline_space(i+1,commandline);
                isspace =1;
                i++;
            }
            
            continue;
        }
        if(character == '2' && commandline->cmd_line[i+1] == '>'){
            if(!isdup2){
                isdup2=1;
                if(!isspace){
                    commandline_space(i,commandline);
                    i++;
                }
                isspace = 0;
            }
            continue;
            // else{
            //     isdup2 = 0;
            //     commandline_space(i+1,commandline);
            //     isspace =1;
            //     i++;
            // }
        }
        if(character == '&' || character == ';' || character == '|' || character == '(' || character == ')' || character == ')' || character == '!'){
            // #ifdef ISDEV
            // printf("case a\n");
            // #endif
            // printf("%c\n",character);
            if((!isspace) && (!isdup2)){
                commandline_space(i,commandline);
                i++;
            }
            isdup2 = 0;
            isspace =1;
            commandline_space(i+1,commandline);
            i++;
            continue;
        }
        isspace = 0;
        if(isdup2){
            isdup2 = 0;
            commandline_space(i,commandline);
            i++;
        }
    }
    // commandline->cmd_end = commandline->cmd_line_size;
    // #ifdef ISDEV
    // printf("parse done \n");
    // #endif
}

//space from front of index
void commandline_space(int index, struct command_line * commandline){
    for(int j = commandline->cmd_line_size ; j > index ; j--){
        commandline->cmd_line[j] = commandline->cmd_line[j-1];
    }
    commandline->cmd_line[index] = 0;
    commandline->cmd_line[++(commandline->cmd_line_size)] = 0;
}


// typedef struct cmd_line{
//     int cmd_start;
//     int cmd_end;
//     int cmd_line_size;

//     int res_idx;

//     char cmd_line[BUFSIZE];
    
//     char * result[BUFSIZE];
// } cmd_line;

//
// int get_next_command(int index, cmd_line * commandline){
//     if()
// }


