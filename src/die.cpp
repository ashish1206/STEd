#include<stdlib.h>
#include<stdio.h>
#include"function_declare.h"


void die(const char* s){
	clearTerminal();
	perror(s);
	exit(1);
}