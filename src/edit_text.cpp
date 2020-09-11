#include<string.h>
#include<stdlib.h>
#include"global_var.h"
#include"function_declare.h"

void editorRowInsertChar(eRow *row, int pos, char c){
    row->str = (char *)realloc(row->str, row->size + 2);
    memmove(&row->str[pos+1], &row->str[pos], row->size - pos + 1);
    row->str[pos] = c;
    row->size++;
    editorUpdateRow(row);
    E.dirty++;
}

void editorInsertChar(char c){
    if(E.cursorY == E.numRows){
        editorAppendRow((char *)"", 0);
    }
    editorRowInsertChar(&E.row[E.cursorY], E.cursorX, c);
    E.cursorX++;
    E.dirty++;
}