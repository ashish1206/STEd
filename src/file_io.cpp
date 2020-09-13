#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
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

void editorInsertRow(int pos, char *str, size_t len){
    if(pos<0 || pos>E.numRows)return;
    E.row = (eRow *)realloc(E.row, sizeof(eRow)*(E.numRows + 1));
    memmove(&E.row[pos + 1], &E.row[pos], sizeof(eRow)*(E.numRows - pos));
    E.row[pos].str = (char *)malloc(len + 1);
    E.row[pos].size = len;
    memcpy(E.row[pos].str, str, len);
    E.row[pos].str[len] = '\0';
    E.numRows++;
    E.row[pos].rSize = 0;
    E.row[pos].render = NULL;
    editorUpdateRow(&E.row[pos]);
}

void editorOpen(char *fileName){
    free(E.filename);
    E.filename = strdup(fileName);
    FILE *fp = fopen(fileName, "r");
    if(!fp)die("File");
    char *line = NULL;
    size_t linecap = 0;
    ssize_t len;
    while((len = getline(&line, &linecap, fp)) != -1){
        while(len > 0 && (line[len-1] == '\n' || line[len-1] == '\r')){
            len--;
        }
        editorInsertRow(E.numRows, line, len);
    }
    free(line);
    fclose(fp);
}

char * editorRowsToStr(int& bufLen){
    for(int i=0; i<E.numRows; i++){
        bufLen += E.row[i].size+1;
    }

    char *buf = (char *)malloc(bufLen);
    char *ptr = buf;
    for(int i=0; i<E.numRows; i++){
        memcpy(ptr, E.row[i].str, E.row[i].size);
        ptr += E.row[i].size;
        *ptr = '\n';
        ptr++;
    }
    E.dirty = 0;
    return buf;
}

void editorSaveFile(){
    if(E.filename == NULL){
        E.filename = editorPrompt((char *)"Save as : %s");
        if(E.filename == NULL){
            return;
        }
    }

    int len = 0;
    char *buf = editorRowsToStr(len);

    int fd = open(E.filename, O_RDWR | O_CREAT, 0644);
    if(fd != -1){
        if(ftruncate(fd, len) != -1){
            if(write(fd, buf, len) == len){
                close(fd);
                free(buf);
                editorSetStatusMessage("%d bytes written to disk", len);
                E.dirty = 0;
                return;
            }
        }
        close(fd);
    }
    free(buf);
    editorSetStatusMessage("can't save! I/O error");
}