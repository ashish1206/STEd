#include<unistd.h>
#include<stdio.h>
#include<stdarg.h>
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

int editorCursorXToRenderX(eRow *row, int cx){
	int rx = 0;
	for(int i=0; i<cx; i++, rx++){
		if(row->str[i] == '\t'){
			rx += (EDITOR_TAB_STOP - 1) - (rx % EDITOR_TAB_STOP);
		}
	}
	return rx;
}

void editorSetStatusMessage(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(E.statusMsg, sizeof(E.statusMsg), fmt, ap);
  va_end(ap);
  E.status_time = time(NULL);
}

void editorDrawMessageBar(struct buffer *b) {
  bufAppend(b, (char *)"\x1b[K", 3);
  int msglen = strlen(E.statusMsg);
  if (msglen > E.windowCol) msglen = E.windowCol;
  if (msglen && time(NULL) - E.status_time < 5)
    bufAppend(b, E.statusMsg, msglen);
}

void editorStatusbar(struct buffer *b){
	int len = 0;
	bufAppend(b, (char *)"\x1b[7m", 4);
	char status[80];
	len = snprintf(status, sizeof(status), "%.40s %d L %s", 
	E.filename ? E.filename : (char *)"[No File]", E.numRows, 
	E.dirty ? (char *)"(modified)" : "");
	char curPos[80];
	int curPosLen = snprintf(curPos, sizeof(curPos), "%d,%d", E.cursorY+1, E.renderX+1);
	bufAppend(b, status, len);
	while(len++ < E.windowCol - curPosLen){
		bufAppend(b, (char *)" ", 1);
	}
	bufAppend(b, curPos, curPosLen);
	bufAppend(b, (char *)"\x1b[m", 3);
	bufAppend(b, (char *)"\r\n", 2);
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
			} else{
				bufAppend(b, (char *)"~", 1);
			}
		}
		else{
			int len = E.row[fileRow].rSize - E.colOffset;
			if(len < 0)len = 0;
			if(len > E.windowCol)len = E.windowCol;
			bufAppend(b, &E.row[fileRow].render[E.colOffset], len);
		}
		bufAppend(b, (char *)"\x1b[K", 3);
		bufAppend(b, (char *)"\r\n", 2);
	}
}

void editorScroll(){
	E.renderX = 0;
	if(E.cursorY < E.numRows){
		E.renderX = editorCursorXToRenderX(&E.row[E.cursorY], E.cursorX);
	}
	if(E.cursorY < E.rowOffset){
		E.rowOffset = E.cursorY; 
	}
	if(E.cursorY >= E.rowOffset + E.windowRow){
		E.rowOffset = E.cursorY - E.windowRow +1;
	}
	if(E.renderX < E.colOffset){
		E.colOffset = E.renderX;
	}
	if(E.renderX >= E.colOffset + E.windowCol){
		E.colOffset = E.renderX - E.windowCol +1;
	}
}

void editorRefreshScreen(){
	editorScroll();
	struct buffer b = {NULL, 0};
	clearTerminal(&b);
	editorDrawEdges(&b);
	editorStatusbar(&b);
	editorDrawMessageBar(&b);
	char buf[32];
	snprintf(buf, sizeof(buf), (char *)"\x1b[%d;%dH", (E.cursorY - E.rowOffset)+1, (E.renderX - E.colOffset)+1);
	bufAppend(&b, buf, strlen(buf));
	// bufAppend(&b, (char*)"\x1b[H", 3);
	write(STDIN_FILENO, b.str, b.len);
	bufFree(&b);
}