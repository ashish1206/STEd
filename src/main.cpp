#include<unistd.h>
#include<stdio.h>
#include<termios.h>
#include<ctype.h>
#include<stdlib.h>
#include<errno.h>
#include"function_declare.h"
#include"global_var.h"

struct editorConfig E;

void initEditor(){
	E.cursorX = 0;
	E.cursorY = 0;
	E.numRows = 0;
	E.row = NULL;
	if(getWindowSize(E.windowRow, E.windowCol) == -1)die("getWindowSize");
}

int main(int argc, char *args[]){
	enableRawMode();
	initEditor();
	if(argc >= 2){
		editorOpen(args[1]);
	}

	while(true){
		editorRefreshScreen();
		editorProcessKeyPress();
	}

	return 0;
}
