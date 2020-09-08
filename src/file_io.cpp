#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include"global_var.h"
#include"function_declare.h"

void editorOpen(char *fileName){
    FILE *fp = fopen(fileName, "r");
    if(!fp)die("File");
    char *line = NULL;
    size_t linecap = 0;
    ssize_t len;
    len = getline(&line, &linecap, fp);
    if(len != -1){
        while(len > 0 && (line[len-1] == '\n' || line[len-1] == '\r')){
            len--;
        }
        E.row.str = (char *)malloc(len + 1);
        memcpy(E.row.str, line, len);
        E.row.size = len;
        E.row.str[len] = '\0';
        E.numRows = 1;
    }
    free(line);
    fclose(fp);
}