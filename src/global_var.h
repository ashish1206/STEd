#include<termios.h>
#define EDITOR_TAB_STOP 8
typedef struct edtRow{
	int size;
	char *str;
	int rSize;
	char *render;
} eRow;

struct editorConfig{
	int cursorX, cursorY;
	int renderX;
	int windowRow, windowCol;
	eRow *row;
	int numRows;
	int rowOffset;
	int colOffset;
	char *filename;
	struct termios orig_termios;
};

extern struct editorConfig E;