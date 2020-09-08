#include<termios.h>
struct editorConfig{
	int cursorX, cursorY;
	int windowRow, windowCol;
	struct termios orig_termios;
};

extern struct editorConfig E;