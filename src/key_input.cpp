#include<unistd.h>
#include<errno.h>
#include<stdlib.h>
#include"function_declare.h"
#define CTRL_KEY(k) (k & 0x1f)

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
	}
}