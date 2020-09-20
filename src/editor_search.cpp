#include<string.h>
#include<stdlib.h>
#include"function_declare.h"
#include"global_var.h"
#define CTRL_KEY(k) (k & 0x1f)

bool searchForward(char *query){
    bool found = false;
    int cx=E.cursorX+1,cy=E.cursorY;
    for(int i=cy; i<E.numRows; i++){
        eRow *row = &E.row[i];
        char *match = strstr(&row->str[cx], query);
        if(match != NULL){
            E.cursorX = match - row->str;
            E.cursorY = i;
            E.rowOffset = E.numRows;
            found = true;
            break;
        }
        cx=0;
    }
    return found;
}

char * reversStr(char *s){
    int len = strlen(s);
    char *str = (char *)malloc(len);
    for(int i=0; s[i]!='\0'; i++){
        str[len-i-1] = s[i];
    }
    str[len] = '\0';
    return str;
}

bool searchBackward(char *query){
    bool found = false;
    int cx=E.cursorX,cy=E.cursorY;
    for(int i=cy; i>=0; i--){
        eRow *row = &E.row[i];
        char *revRow = reversStr(row->str);
        char *revQ = reversStr(query);
        char *match = strstr(&revRow[row->size-cx-1], revQ);
        if(match != NULL){
            E.cursorX = row->size - (match - revRow + strlen(query));
            E.cursorY = i;
            E.rowOffset = E.numRows;
            found = true;
            break;
        }
        if(i>0)
        cx=E.row[i-1].size-1;
    }
    return found;
}
void editorSearch(){
    char * query = editorPrompt((char *)"Search : %s");
    if(query == NULL)return;
    bool first = searchForward(query);
    if(first){
        editorSetStatusMessage("ctrl+q : quit search, > : next, < : previous");
    }
    else{
        editorSetStatusMessage("no result found");
        return;
    }
    while(true){
        editorRefreshScreen();
        int ch = editorReadKey();
        if(ch == ARROW_LEFT){
            bool found = searchBackward(query);
            if(found){
                editorSetStatusMessage("ctrl+q : quit search, > : next, < : previous");
            }
            else{
                editorSetStatusMessage("Reached top, ctrl+q : quit search, > : next");
            }
        }
        else if(ch == ARRROW_RIGHT){
            bool found = searchForward(query);
            if(found){
                editorSetStatusMessage("ctrl+q : quit search, > : next, < : previous");
            }
            else{
                editorSetStatusMessage("Reached bottom, ctrl+q : quit search, < : previous");
            }
        }
        else if(ch == CTRL_KEY('q')){
            editorSetStatusMessage((char *)"");
            return;
        }
    }
    free(query);
}