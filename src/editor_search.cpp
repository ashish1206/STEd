#include<string.h>
#include<stdlib.h>
#include"function_declare.h"
#include"global_var.h"

void editorSearch(){
    char * query = editorPrompt((char *)"Search : %s");
    if(query == NULL)return;
    bool found = false;
    for(int i=0; i<E.numRows; i++){
        eRow *row = &E.row[i];
        char *match = strstr(row->str, query);
        if(match != NULL){
            E.cursorX = match - row->str;
            E.cursorY = i;
            found = true;
            E.rowOffset = E.numRows;
            break;
        }
    }
    free(query);
    found?editorSetStatusMessage("result found at %d, %d", E.cursorY+1, E.cursorX+1):
    editorSetStatusMessage("no result found");
}