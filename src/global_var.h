#include<termios.h>
#include<time.h>
#define EDITOR_TAB_STOP 8
#define EDITOR_QUIT_TIME 1
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
	time_t status_time;
	char statusMsg[80];
	struct termios orig_termios;
	int dirty;
};

extern struct editorConfig E;

enum editorKey{
	BACKSPACE = 127,
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