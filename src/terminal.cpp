#include<termios.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/ioctl.h>
#include"function_declare.h"
#include"global_var.h"

int getWindowSize(int& row, int& col){
	struct winsize ws;
	if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0){
		return -1;
	}
	else{
		row = ws.ws_row;
		col = ws.ws_col;
		return 0;
	}
}

void enableRawMode(){
	if(tcgetattr(STDIN_FILENO, &E.orig_termios) == -1)die("tcgetattr");
	struct termios raw = E.orig_termios;
	atexit(disableRawMode);
	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 1;
	if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)die("tcsetattr");
}

void disableRawMode(){
	if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == -1)die("tcsetattr");
}