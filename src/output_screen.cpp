#include<unistd.h>
#include<stdio.h>
#include"function_declare.h"
#include"global_var.h"
#include"buffer.h"
#define EDITOR_VERSION "0.0.1"

void clearTerminal(){
	struct buffer b = {NULL, 0};
	bufAppend(&b, (char *)"\x1b[2J", 4);
	bufAppend(&b, (char *)"\x1b[H", 3);
	write(STDIN_FILENO, b.str, b.len);
	bufFree(&b);
}

void clearTerminal(struct buffer *b){
	bufAppend(b, (char *)"\x1b[?25l", 6);
	// bufAppend(b, (char *)"\x1b[2J", 4);
	bufAppend(b, (char *)"\x1b[H", 3);
	bufAppend(b, (char *)"\x1b[?25h", 6);
}

void editorDrawEdges(struct buffer *b){
	for(int y=0;y<E.windowRow;y++){
		if(y >= E.numRows){
			if (y == E.windowRow / 3) {
			char welcome[80];
			int welcomelen = snprintf(welcome, sizeof(welcome),
				"Simple editor -- version %s", EDITOR_VERSION);
			if (welcomelen > E.windowRow) welcomelen = E.windowRow;
			int padding = (E.windowCol - welcomelen)/2;
			if(padding){
				bufAppend(b, (char *)"~", 1);
				padding--;
			}
			while(padding--){
				bufAppend(b, (char *)" ", 1);
			}
			bufAppend(b, welcome, welcomelen);
			} else {
				bufAppend(b, (char *)"~", 1);
			}
		}
		else{
			int len = E.row.size;
			if(len > E.windowCol)len = E.windowCol;
			bufAppend(b, E.row.str, len);
		}
		bufAppend(b, (char *)"\x1b[K", 3);
		if(y < E.windowRow-1)
		bufAppend(b, (char *)"\r\n", 2);
	}
}

void editorRefreshScreen(){
	struct buffer b = {NULL, 0};
	clearTerminal(&b);
	editorDrawEdges(&b);
	char buf[32];
	snprintf(buf, sizeof(buf), (char *)"\x1b[%d;%dH", E.cursorY+1, E.cursorX+1);
	bufAppend(&b, buf, strlen(buf));
	// bufAppend(&b, (char*)"\x1b[H", 3);
	write(STDIN_FILENO, b.str, b.len);
	bufFree(&b);
}