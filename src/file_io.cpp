#include<stdlib.h>
#include<string.h>
#include"global_var.h"

void editorOpen(){
    char *line = (char *)"Hello World!";
    ssize_t len = 12;
    E.row.str = (char *)malloc(len + 1);
    memcpy(E.row.str, line, len);
    E.row.size = len;
    E.row.str[len] = '\0';
    E.numRows = 1;
}