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

void editorStrDel(eRow *row){
    free(row->str);
    free(row->render);
}

void editorRowDel(int pos){
    if(pos == E.numRows)return;
    editorStrDel(&E.row[pos]);
    memmove(&E.row[pos], &E.row[pos + 1], sizeof(eRow)*(E.numRows - pos - 1));
    E.numRows--;
    E.dirty++;
}

void editorRowAppendStr(eRow *row, char *str, size_t len){
    row->str = (char *)realloc(row->str, row->size + len + 1);
    memcpy(&row->str[row->size], str, len);
    row->size += len;
    row->str[row->size] = '\0';
    editorUpdateRow(row);
    E.dirty++;
}

void editorRowDeleteChar(eRow *row, int pos){
    memmove(&row->str[pos], &row->str[pos+1], row->size - pos);
    row->size--;
    editorUpdateRow(row);
    E.dirty++;
}

void editorDelChar(){
    if(E.cursorY == E.numRows)return;
    if(E.cursorX == 0 && E.cursorY == 0)return;
    if(E.cursorX > 0){
        editorRowDeleteChar(&E.row[E.cursorY], E.cursorX - 1);
        E.cursorX--;
    }
    else{
        eRow *row = &E.row[E.cursorY - 1];
        char *str = E.row[E.cursorY].str;
        size_t len = E.row[E.cursorY].size;
        E.cursorX = row->size;
        editorRowAppendStr(row, str, len);
        editorRowDel(E.cursorY);
        E.cursorY--;
    }
}