#include<unistd.h>
#include"function_declare.h"
#include"global_var.h"
#include"buffer.cpp"

void clearTerminal(){
	struct buffer b = {NULL, 0};
	bufAppend(&b, (char *)"\x1b[2J", 4);
	bufAppend(&b, (char *)"\x1b[H", 3);
	write(STDIN_FILENO, b.str, b.len);
	bufFree(&b);
}

void clearTerminal(struct buffer *b){
	bufAppend(b, (char *)"\x1b[?25l", 6);
	bufAppend(b, (char *)"\x1b[2J", 4);
	bufAppend(b, (char *)"\x1b[H", 3);
	bufAppend(b, (char *)"\x1b[?25h", 6);
}

void editorDrawEdges(struct buffer *b){
	for(int y=0;y<E.windowCol;y++){
		bufAppend(b, (char *)"~", 1);
		if(y < E.windowCol-1)
		bufAppend(b, (char *)"\r\n", 2);
	}
}

void editorRefreshScreen(){
	struct buffer b = {NULL, 0};
	clearTerminal(&b);
	editorDrawEdges(&b);
	bufAppend(&b, (char*)"\x1b[H", 3);
	write(STDIN_FILENO, b.str, b.len);
	bufFree(&b);
}