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
	ARROW_DOWN,
	PAGE_UP,
	PAGE_DOWN,
	HOME_KEY,
	END_KEY,
	DEL_KEY
};

void editorMoveCursor(int key){
	eRow *row = E.cursorY >= E.numRows ? NULL : &E.row[E.cursorY];
	switch(key){
		case ARROW_UP:
			if(E.cursorY != 0)
			E.cursorY--;
			break;
		case ARRROW_RIGHT:
			if(row && E.cursorX < row->size)
			E.cursorX++;
			else if(row && E.cursorY < E.numRows){
				E.cursorY++;
				E.cursorX = 0;
			}
			break;
		case ARROW_DOWN:
			if(E.cursorY < E.numRows)
			E.cursorY++;
			break;
		case ARROW_LEFT:
			if(E.cursorX != 0)
			E.cursorX--;
			else if(E.cursorY > 0){
				E.cursorY--;
				E.cursorX = E.row[E.cursorY].size;
			}
			break;
	}
	row = E.cursorY >= E.numRows ? NULL : &E.row[E.cursorY];
	int rowlen = row ? row->size : 0;
	if(rowlen < E.cursorX){
		E.cursorX = rowlen;
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
			if(seq[0] >= '0' && seq[1]<= '9'){
				if(read(STDIN_FILENO, &seq[2], 1) != 1)return '\x1b';
				if(seq[2] == '~'){
					switch (seq[1]){
					case '1':
					case '7':
						return HOME_KEY;
					case '3':
						return DEL_KEY;
					case '4':
					case '8':
						return END_KEY;
					case '5':
						return PAGE_UP;
					case '6':
						return PAGE_DOWN;
					}
				}
			}
			else{
				switch (seq[1]){
				case 'A':
					return ARROW_UP;
				case 'B':
					return ARROW_DOWN;
				case 'C':
					return ARRROW_RIGHT;
				case 'D':
					return ARROW_LEFT;
				case 'H':
					return HOME_KEY;
				case 'F':
					return END_KEY;
				}
			}
		}
		else if(seq[0] == '0'){
			if(read(STDIN_FILENO, &seq[1], 1) != 1)return '\x1b';
			switch (seq[1]){
			case 'H':
				return HOME_KEY;
			case 'F':
				return END_KEY;
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
		case PAGE_UP:
		case PAGE_DOWN:
		{
			int moves = E.windowRow;
			while(moves--){
				editorMoveCursor(c == PAGE_UP? ARROW_UP: ARROW_DOWN);
			}
			editorScroll();
			if(E.cursorY < E.numRows)
			E.rowOffset = E.cursorY;
		}
		break;
		case HOME_KEY:
			E.cursorX = 0;
		break;
		case END_KEY:
			if(E.cursorY < E.numRows)
			E.cursorX = E.row[E.cursorY].size;
		break;
		case ARROW_LEFT:
		case ARROW_DOWN:
		case ARRROW_RIGHT:
		case ARROW_UP:
			editorMoveCursor(c);
			break;
		default:
			editorInsertChar(c);
			break;
	}
}