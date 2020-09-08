#include<unistd.h>
#include<errno.h>
#include<stdlib.h>
#include<stdio.h>
#include<ctype.h>
#include"function_declare.h"
#include"global_var.h"
#define CTRL_KEY(k) (k & 0x1f)

enum editorKey{
	ARROW_LEFT = 1000,
	ARRROW_RIGHT,
	ARROW_UP,
	ARROW_DOWN
};

void editorMoveCursor(int key){
	switch(key){
		case ARROW_UP:
			E.cursorY--;
			break;
		case ARRROW_RIGHT:
			E.cursorX++;
			break;
		case ARROW_DOWN:
			E.cursorY++;
			break;
		case ARROW_LEFT:
			E.cursorX--;
			break;
	}
}

int editorReadKey(){
	int nread;
	char c;
	while((nread = read(STDIN_FILENO, &c, 1)) != 1){
		if(nread == -1 && errno != EAGAIN)die("read");
	}
	if(c == '\x1b'){
		char seq[3];
		if(read(STDIN_FILENO, &seq[0], 1) != 1)return '\x1b';
		if(read(STDIN_FILENO, &seq[1], 1) != 1)return '\x1b';
		if(seq[0] == '['){
			switch (seq[1]){
			case 'A':
				return ARROW_UP;
			case 'B':
				return ARROW_DOWN;
			case 'C':
				return ARRROW_RIGHT;
			case 'D':
				return ARROW_LEFT;
			}
		}
		return '\x1b';
	}
	else{
		return c;
	}
}

void editorProcessKeyPress(){
	int c = editorReadKey();
	switch(c){
		case CTRL_KEY('q') :
			clearTerminal();
			exit(1);
			break;
		case ARROW_LEFT:
		case ARROW_DOWN:
		case ARRROW_RIGHT:
		case ARROW_UP:
			editorMoveCursor(c);
			break;
	}
}