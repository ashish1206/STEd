#include<stdlib.h>
#include<ctype.h>
#include"function_declare.h"
#include"global_var.h"

char * editorPrompt(char * promptMsg){
    size_t buflen = 0, bufsize = 128;
    char *buf = (char *)malloc(bufsize);
    buf[0] = '\0';

    while(true){
        editorSetStatusMessage(promptMsg, buf);
        editorRefreshScreen();
        char c = editorReadKey();
        if(c == '\r'){
            if(buflen != 0){
                editorSetStatusMessage((char *)"File saved");
                return buf;
            }
            else{
                return NULL;
            }
        }
        else if(c == '\x1b'){
            editorSetStatusMessage((char *)"");
            free(buf);
            return NULL;
        }
        else if(c == BACKSPACE || c == DEL_KEY){
            if(buflen > 0)buf[--buflen] = '\0';
        }
        else if(!iscntrl(c) && c < 128){
            if(buflen+1 == bufsize){
                buf = (char *)realloc(buf, bufsize*2);
                bufsize *= 2;
            }
            buf[buflen++] = c;
            buf[buflen] = '\0';
        }
    }
}