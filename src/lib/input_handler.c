#include "filecontroller.h"

// return num : index of ex idx(for arrow) | -1 : input nothing | 
int input_handler(char commands[COMMAND_QUEUE_SIZE][BUFSIZE], int * cmd_idx){
    int result = -1;
    int ch_idx = strlen(commands[*cmd_idx]);
    int isesc = 0;
    int isarrow = 0;
    while(1){
        // printf("\n%s %d\n",commands[*cmd_idx],ch_idx);
        unsigned char key =getch();
        // #ifdef ISDEV
        // printf("%c %d\n",key,key);
        // printf("%d here\n",key);
        // #endif
        if(key == ASCII_ESC){
            isesc = 1;
            continue;
        }
        if(key == ASCII_SQ_BRACKET_OP && isesc == 1){
            isarrow = 1;
            continue;
        }
        if(isarrow){
            // #ifdef ISDEV
            // printf("arrow %d\n",key);
            // #endif
            int isupdown = 0;
            if(key == ASCII_UP_A){
                (*cmd_idx)--;
                isupdown = 1;
            }
            if(key == ASCII_DOWN_B){
                (*cmd_idx)++;
                isupdown = 1;
            }
            if(isupdown){
                for(int i = 0; i < ch_idx; i++){
                    write_on_fd(STDOUT_FILENO,"\b \b");
                }
                break;
            }
            if(key == ASCII_LEFT_D){
                if(--ch_idx < 0)ch_idx = 0;
                else write_on_fd(STDOUT_FILENO,"\b");
            }
            if(key == ASCII_RIGHT_C){
                if(++ch_idx > strlen(commands[*cmd_idx]))ch_idx = strlen(commands[*cmd_idx]);
                else{
                    putchar(commands[*cmd_idx][ch_idx-1]);
                }
            }
            isesc = 0;
            isarrow = 0;
            continue;
        }
        if(key == ASCII_BACKSPACE){
            if(commands[*cmd_idx][ch_idx] == 0){
                if(--ch_idx < 0)ch_idx = 0;
                write_on_fd(STDOUT_FILENO,"\b \b");
                commands[*cmd_idx][ch_idx] = 0;
            }else{
                if(--ch_idx < 0)ch_idx = 0;
                for(int i = ch_idx; i < strlen(commands[*cmd_idx]); i++){
                    commands[*cmd_idx][i] = commands[*cmd_idx][i+1];
                }
                write_on_fd(STDOUT_FILENO,"\b");
                write_on_fd(STDOUT_FILENO,commands[*cmd_idx]+ch_idx);
                write_on_fd(STDOUT_FILENO," ");
                for(int i = strlen(commands[*cmd_idx])+1; i > ch_idx; i--){
                    write_on_fd(STDOUT_FILENO,"\b");
                }
            }
                
            
            continue;
        }
        if(key == ASCII_CTRL_C){
            write_on_fd(STDOUT_FILENO,"^C");
            commands[*cmd_idx][0] = 0;
            break;
        }
        if(key == ASCII_ENTER){
            result = ch_idx;
            break;
        }
        isesc = 0;
        isarrow = 0;
        // need to perror(out of index)
        if(ch_idx+1 < BUFSIZE){
            if(commands[*cmd_idx][ch_idx] == 0){
                commands[*cmd_idx][ch_idx++] = key;
                commands[*cmd_idx][ch_idx] = 0;
                putchar(key);
            }else{
                for(int i = strlen(commands[*cmd_idx])+1; i > ch_idx; i--){
                    commands[*cmd_idx][i] = commands[*cmd_idx][i-1];
                }
                commands[*cmd_idx][ch_idx] = key;
                write_on_fd(STDOUT_FILENO,commands[*cmd_idx]+ch_idx++);
                for(int i = strlen(commands[*cmd_idx]); i > ch_idx; i--){
                    write_on_fd(STDOUT_FILENO,"\b");
                }
            }
        }
    }
    return result;
}
/* reads from keypress, doesn't echo */
int getch(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}

/* reads from keypress, echoes */
int getche(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}