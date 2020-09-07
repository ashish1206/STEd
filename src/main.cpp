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
	if(getWindowSize(E.windowRow, E.windowCol) == -1)die("getWindowSize");
}

int main(){
	enableRawMode();
	initEditor();

	while(true){
		editorRefreshScreen();
		editorProcessKeyPress();
	}

	return 0;
}
