#pragma once
#include<termios.h>
struct editorConfig{
	int windowRow;
	int windowCol;
	struct termios orig_termios;
};

extern struct editorConfig E;