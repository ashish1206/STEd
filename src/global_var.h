#include<termios.h>
typedef struct edtRow{
	int size;
	char *str;
} eRow;

struct editorConfig{
	int cursorX, cursorY;
	int windowRow, windowCol;
	eRow *row;
	int numRows;
	int rowOffset;
	struct termios orig_termios;
};

extern struct editorConfig E;