#include<unistd.h>
#include<errno.h>
#include<stdlib.h>
#include"function_declare.h"
#include"global_var.h"
#define CTRL_KEY(k) (k & 0x1f)

void editorMoveCursor(char key){
	switch(key){
		case 'w':
			E.cursorY--;
			break;
		case 'd':
			E.cursorX++;
			break;
		case 's':
			E.cursorY++;
			break;
		case 'a':
			E.cursorX--;
			break;
	}
}

char editorReadKey(){
	int nread;
	char c;
	while((nread = read(STDIN_FILENO, &c, 1)) != 1){
		if(nread == -1 && errno != EAGAIN)die("read");
	}
	return c;
}

void editorProcessKeyPress(){
	char c = editorReadKey();
	switch(c){
		case CTRL_KEY('q') :
			clearTerminal();
			exit(1);
			break;
		case 'a':
		case 's':
		case 'd':
		case 'w':
			editorMoveCursor(c);
			break;
	}
}