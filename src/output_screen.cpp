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
		int fileRow = y+E.rowOffset;
		if(fileRow >= E.numRows){
			if (y == E.windowRow/3 && E.numRows == 0) {
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
			int len = E.row[fileRow].size - E.colOffset;
			if(len < 0)len = 0;
			if(len > E.windowCol)len = E.windowCol;
			bufAppend(b, &E.row[fileRow].str[E.colOffset], len);
		}
		bufAppend(b, (char *)"\x1b[K", 3);
		if(y < E.windowRow-1)
		bufAppend(b, (char *)"\r\n", 2);
	}
}

void editorScroll(){
	if(E.cursorY < E.rowOffset){
		E.rowOffset = E.cursorY; 
	}
	if(E.cursorY >= E.rowOffset + E.windowRow){
		E.rowOffset = E.cursorY - E.windowRow +1;
	}
	if(E.cursorX < E.colOffset){
		E.colOffset = E.cursorX;
	}
	if(E.cursorX >= E.colOffset + E.windowCol){
		E.colOffset = E.cursorX - E.windowCol +1;
	}
}

void editorRefreshScreen(){
	editorScroll();
	struct buffer b = {NULL, 0};
	clearTerminal(&b);
	editorDrawEdges(&b);
	char buf[32];
	snprintf(buf, sizeof(buf), (char *)"\x1b[%d;%dH", (E.cursorY - E.rowOffset)+1, (E.cursorX - E.colOffset)+1);
	bufAppend(&b, buf, strlen(buf));
	// bufAppend(&b, (char*)"\x1b[H", 3);
	write(STDIN_FILENO, b.str, b.len);
	bufFree(&b);
}