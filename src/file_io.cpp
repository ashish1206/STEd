#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include"global_var.h"
#include"function_declare.h"

void editorAppendRow(char *str, size_t len){
    E.row = (eRow *)realloc(E.row, sizeof(eRow)*(E.numRows + 1));
    int lastRow = E.numRows;
    E.row[lastRow].str = (char *)malloc(len + 1);
    E.row[lastRow].size = len;
    memcpy(E.row[lastRow].str, str, len);
    E.row[lastRow].str[len] = '\0';
    E.numRows++;
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