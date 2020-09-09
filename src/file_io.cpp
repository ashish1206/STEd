#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include"global_var.h"
#include"function_declare.h"

void editorUpdateRow(eRow *row){
    int tabs = 0;
    for(int i=0;i<row->size;i++){
        if(row->str[i] == '\t')tabs++;
    }
    free(row->render);
    row->render = (char *)malloc(row->size + tabs*(EDITOR_TAB_STOP - 1) +1);
    int id = 0;
    for(int i = 0; i<row->size; i++){
        if(row->str[i] == '\t'){
            row->render[id++] = ' ';
            while(id%EDITOR_TAB_STOP != 0){
                row->render[id++] = ' ';
            }
        }
        else{
            row->render[id++] = row->str[i];
        }
    }
    row->render[id] = '\0';
    row->rSize = id;
}

void editorAppendRow(char *str, size_t len){
    E.row = (eRow *)realloc(E.row, sizeof(eRow)*(E.numRows + 1));
    int lastRow = E.numRows;
    E.row[lastRow].str = (char *)malloc(len + 1);
    E.row[lastRow].size = len;
    memcpy(E.row[lastRow].str, str, len);
    E.row[lastRow].str[len] = '\0';
    E.numRows++;
    E.row[lastRow].rSize = 0;
    E.row[lastRow].render = NULL;
    editorUpdateRow(&E.row[lastRow]);
}

void editorOpen(char *fileName){
    FILE *fp = fopen(fileName, "r");
    if(!fp)die("File");
    char *line = NULL;
    size_t linecap = 0;
    ssize_t len;
    while((len = getline(&line, &linecap, fp)) != -1){
        while(len > 0 && (line[len-1] == '\n' || line[len-1] == '\r')){
            len--;
        }
        editorAppendRow(line, len);
    }
    free(line);
    fclose(fp);
}